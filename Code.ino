#include <AFMotor.h>
#include <SoftwareSerial.h>
#include <DFRobot_MuVisionSensor.h>
#include <Servo.h>

#define IR1 A0
#define IR2 A1
#define IR3 A2
#define IR4 A3
#define IR5 A4

#define SERVO_PIN 10

int THRESHOLD = 300;

AF_DCMotor FL(1);
AF_DCMotor BL(2);
AF_DCMotor FR(3);
AF_DCMotor BR(4);

int SPEED = 110;

SoftwareSerial muSerial(2, A5);
DFRobot_MuVisionSensor mu(0x60);

Servo gateServo;

int mode = 0;

bool ignoreStopUntilWhite = false;

void setup() {
  Serial.begin(9600);
  muSerial.begin(9600);

  FL.setSpeed(SPEED);
  BL.setSpeed(SPEED);
  FR.setSpeed(SPEED);
  BR.setSpeed(SPEED);

  while (mu.begin(&muSerial, kSerialMode) != 0) {
    Serial.println("MU Vision not connected...");
    delay(1000);
  }

  Serial.println("MU Vision connected.");
  mu.visionBegin(VISION_TRAFFIC_CARD_DETECT);
}

void stopAll() {
  FL.run(RELEASE);
  BL.run(RELEASE);
  FR.run(RELEASE);
  BR.run(RELEASE);
}

void forwardMove() {
  FL.run(FORWARD);
  BL.run(FORWARD);
  FR.run(FORWARD);
  BR.run(FORWARD);
}

void moveRight() {
  FL.setSpeed(SPEED);
  BL.setSpeed(SPEED);
  FR.setSpeed(SPEED);
  BR.setSpeed(SPEED);

  FL.run(FORWARD);
  BL.run(BACKWARD);
  FR.run(BACKWARD);
  BR.run(FORWARD);
}

void slightLeft() {
  FL.setSpeed(80);
  BL.setSpeed(80);
  FR.setSpeed(130);
  BR.setSpeed(130);
  forwardMove();
}

void slightRight() {
  FL.setSpeed(130);
  BL.setSpeed(130);
  FR.setSpeed(80);
  BR.setSpeed(80);
  forwardMove();
}

void sharpLeft() {
  FL.run(BACKWARD);
  BL.run(BACKWARD);
  FR.run(FORWARD);
  BR.run(FORWARD);
}

void sharpRight() {
  FL.run(FORWARD);
  BL.run(FORWARD);
  FR.run(BACKWARD);
  BR.run(BACKWARD);
}

void openGate() {
  gateServo.attach(SERVO_PIN);
  gateServo.write(90);
  delay(1000);
  gateServo.detach();
}

void loop() {
  int v1 = analogRead(IR1);
  int v2 = analogRead(IR2);
  int v3 = analogRead(IR3);
  int v4 = analogRead(IR4);
  int v5 = analogRead(IR5);

  int s1 = (v1 < THRESHOLD) ? 1 : 0;
  int s2 = (v2 < THRESHOLD) ? 1 : 0;
  int s3 = (v3 < THRESHOLD) ? 1 : 0;
  int s4 = (v4 < THRESHOLD) ? 1 : 0;
  int s5 = (v5 < THRESHOLD) ? 1 : 0;

  bool allBlack = (s1 == 1 && s2 == 1 && s3 == 1 && s4 == 1 && s5 == 1);
  bool allWhite = (s1 == 0 && s2 == 0 && s3 == 0 && s4 == 0 && s5 == 0);

  Serial.print(s1); Serial.print(" ");
  Serial.print(s2); Serial.print(" ");
  Serial.print(s3); Serial.print(" ");
  Serial.print(s4); Serial.print(" ");
  Serial.println(s5);

  if (mode == 3) {
    stopAll();
    return;
  }

  if (mode == 2) {
    if (allWhite) {
      Serial.println("ALL WHITE DETECTED -> STOP + OPEN GATE");
      stopAll();
      delay(300);

      openGate();

      mode = 3;
      return;
    }

    moveRight();
    delay(40);
    return;
  }

  if (mode == 1) {
    stopAll();

    int detected = mu.getValue(VISION_TRAFFIC_CARD_DETECT, kStatus);

    if (detected) {
      int label = mu.getValue(VISION_TRAFFIC_CARD_DETECT, kLabel);

      Serial.print("Traffic card label: ");
      Serial.println(label);

      if (label == 3) {
        Serial.println("RIGHT CARD -> MOVE RIGHT UNTIL ALL WHITE");
        mode = 2;
        delay(300);
        return;
      } 
      else {
        Serial.println("FORWARD/LEFT CARD -> CONTINUE LINE FOLLOWING");
        mode = 0;
        ignoreStopUntilWhite = true;
        return;
      }
    }

    return;
  }

  if (ignoreStopUntilWhite) {
    if (!allBlack) {
      ignoreStopUntilWhite = false;
    }
  }

  if (allBlack && !ignoreStopUntilWhite) {
    Serial.println("ALL BLACK -> STOP AND SCAN CARD");
    stopAll();
    mode = 1;
    delay(500);
    return;
  }

  if (s2 == 1 && s3 == 1 && s4 == 0) {
    slightLeft();
  }
  else if (s3 == 1 && s4 == 1 && s2 == 0) {
    slightRight();
  }
  else if (s3 == 1 && s2 == 1) {
    FL.setSpeed(SPEED);
    BL.setSpeed(SPEED);
    FR.setSpeed(SPEED);
    BR.setSpeed(SPEED);
    forwardMove();
  }
  else if (s1 == 1) {
    sharpLeft();
  }
  else if (s5 == 1) {
    sharpRight();
  }
  else {
    stopAll();
  }

  delay(40);
}