#include <AFMotor.h>
#include <SoftwareSerial.h>
#include <DFRobot_MuVisionSensor.h>

#define IR1 A0
#define IR2 A1
#define IR3 A2
#define IR4 A3
#define IR5 A4

int THRESHOLD = 300;

AF_DCMotor FL(1);
AF_DCMotor BL(2);
AF_DCMotor FR(3);
AF_DCMotor BR(4);

int SPEED = 110;

SoftwareSerial muSerial(2, A5);
DFRobot_MuVisionSensor mu(0x60);

bool stopped = false;
bool actionDone = false;

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

  Serial.print(s1); Serial.print(" ");
  Serial.print(s2); Serial.print(" ");
  Serial.print(s3); Serial.print(" ");
  Serial.print(s4); Serial.print(" ");
  Serial.println(s5);

  if (s1 == 1 && s2 == 1 && s3 == 1 && s4 == 1 && s5 == 1 && stopped == false) {
    stopAll();
    stopped = true;
    Serial.println("STOPPED: All IR black");
    delay(500);
  }

  if (stopped == true && actionDone == false) {
    stopAll();

    int detected = mu.getValue(VISION_TRAFFIC_CARD_DETECT, kStatus);

    if (detected) {
      int label = mu.getValue(VISION_TRAFFIC_CARD_DETECT, kLabel);

      Serial.print("Traffic card label: ");
      Serial.println(label);

      if (label == 3) {
        Serial.println("RIGHT card detected -> Move right");
        moveRight();
        delay(1500);
        stopAll();
      } 
      else {
        Serial.println("Other card detected -> Move forward");
        FL.setSpeed(SPEED);
        BL.setSpeed(SPEED);
        FR.setSpeed(SPEED);
        BR.setSpeed(SPEED);
        forwardMove();
        delay(1500);
        stopAll();
      }

      actionDone = true;
    }

    return;
  }

  if (stopped == true) {
    stopAll();
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