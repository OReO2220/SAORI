#include <ESP32Servo.h>

#define hallpin 33 //INA:33 INB:32
#define ledPin 26 //OUTA:26 OUTB:13
#define moterPin 13
#define on LOW
#define off HIGH
Servo myservo;

void setup() {
  Serial.begin(115200);
  pinMode(hallpin, INPUT);
  pinMode(ledPin, OUTPUT);
  myservo.attach(moterPin, 700, 2300);
}

bool ledOn = false; // LEDの状態を記録する変数
bool servoMoved = false; // Servoの動きを記録する変数

void loop() {
  int base = 1650; // ゼロガウス(0G) = 2.5V(2500mV)
  // アナログ入力の値を電圧に変換(mV)
  float voltage = (analogRead(hallpin) / 4096.0) * 3.3 * 1000;
  float gauss = abs((voltage - base) / 3.3); // 1ガウス(1G) = 5mV
  Serial.print(gauss);
  Serial.println("gauss");
  delay(500);

  // 磁石のプラスチック側（N極）をHallセンサーの「Hall」シルク側へ近づけるとgauss値が高くなる。
  if (gauss >= 130) {
    digitalWrite(ledPin, on);
    Serial.println("閉まっています");

    // Servoを動かす
    if (!servoMoved) {
      myservo.write(1300);
      delay(750);
      myservo.write(1500);
      servoMoved = true;
    }
  } else if (gauss < 130) {
    digitalWrite(ledPin, off);
    Serial.println("開いています");
    delay(1000);
    servoMoved = false; // gaussが130未満のときServoを再び動かすためにリセット
  }
}