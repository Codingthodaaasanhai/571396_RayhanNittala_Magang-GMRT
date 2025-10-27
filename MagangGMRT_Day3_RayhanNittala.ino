/*
  Name: Rayhan Nittala
  NIM: 25/571396/TK/64973
  Project: Magang GMRT Day 3 - Following Axis Servo Motor and Motion Detection
  Description:
    ESP32 + MPU6050_tockn + PIR Sensor to control 5 servo motors
    based on roll, pitch, yaw, and external motion.
*/

#include <Wire.h>
#include <ESP32Servo.h>
#include <MPU6050_tockn.h>

// Create MPU and Servo objects
MPU6050 mpu(Wire);
Servo servo1, servo2, servo3, servo4, servo5;

// Pin definitions
#define PIR_PIN 14
#define SERVO1_PIN 13
#define SERVO2_PIN 12
#define SERVO3_PIN 27
#define SERVO4_PIN 26
#define SERVO5_PIN 25

int pirState = 0;
float roll, pitch, yaw;

void setup() {
  Serial.begin(115200);
  Wire.begin(21, 22); // SDA=21, SCL=22

  pinMode(PIR_PIN, INPUT);

  servo1.attach(SERVO1_PIN);
  servo2.attach(SERVO2_PIN);
  servo3.attach(SERVO3_PIN);
  servo4.attach(SERVO4_PIN);
  servo5.attach(SERVO5_PIN);
  resetServos();

  // Initialize MPU6050
  mpu.begin();
  mpu.calcGyroOffsets(true); // Optional calibration
  Serial.println("MPU6050 ready!");
}

void loop() {
  pirState = digitalRead(PIR_PIN);

  mpu.update();
  roll  = mpu.getAngleX();
  pitch = mpu.getAngleY();
  yaw   = mpu.getAngleZ();

  roll  = constrain(roll, -90, 90);
  pitch = constrain(pitch, -90, 90);
  yaw   = constrain(yaw, -90, 90);

  if (pirState == HIGH) {
    Serial.println("Motion detected by PIR!");
    allServoAlert();
  } else {
    // Roll: servo1 & servo2 move opposite
    servo1.write(90 - roll);
    servo2.write(90 + roll);

    // Pitch: servo3 & servo4 move together
    servo3.write(90 + pitch);
    servo4.write(90 + pitch);

    // Yaw: servo5 follows then returns
    servo5.write(90 + yaw);
    delay(1000);
    servo5.write(90);
  }

  delay(200);
}

void allServoAlert() {
  for (int i = 1; i <= 5; i++) getServo(i).write(150);
  delay(1000);
  resetServos();
}

Servo& getServo(int n) {
  switch (n) {
    case 1: return servo1;
    case 2: return servo2;
    case 3: return servo3;
    case 4: return servo4;
    default: return servo5;
  }
}

void resetServos() {
  servo1.write(90);
  servo2.write(90);
  servo3.write(90);
  servo4.write(90);
  servo5.write(90);
}
