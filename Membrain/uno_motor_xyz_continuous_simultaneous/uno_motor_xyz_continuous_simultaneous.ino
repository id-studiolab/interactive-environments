#include <AccelStepper.h>

// For Arduino Uno + CNC shield V3
#define MOTOR_X_ENABLE_PIN 8
#define MOTOR_X_STEP_PIN 2
#define MOTOR_X_DIR_PIN 5

#define MOTOR_Y_ENABLE_PIN 8
#define MOTOR_Y_STEP_PIN 3
#define MOTOR_Y_DIR_PIN 6

#define MOTOR_Z_ENABLE_PIN 8
#define MOTOR_Z_STEP_PIN 4
#define MOTOR_Z_DIR_PIN 7

int moveSize = 8000;

AccelStepper motorX(1, MOTOR_X_STEP_PIN, MOTOR_X_DIR_PIN);

AccelStepper motorY(1, MOTOR_Y_STEP_PIN, MOTOR_Y_DIR_PIN);

AccelStepper motorZ(1, MOTOR_Z_STEP_PIN, MOTOR_Z_DIR_PIN);

void setup() {
  pinMode(MOTOR_X_ENABLE_PIN, OUTPUT);

  pinMode(MOTOR_Y_ENABLE_PIN, OUTPUT);

  pinMode(MOTOR_Z_ENABLE_PIN, OUTPUT);

  delay(10);
  Serial.begin(9600);

  motorX.setEnablePin(MOTOR_X_ENABLE_PIN);
  motorX.setPinsInverted(false, false, true);
  motorX.setAcceleration(87000);
  motorX.setMaxSpeed(2000);
  motorX.setSpeed(2000);
  motorX.enableOutputs();

  motorY.setEnablePin(MOTOR_Y_ENABLE_PIN);
  motorY.setPinsInverted(false, false, true);
  motorY.setAcceleration(87000);
  motorY.setMaxSpeed(2000);
  motorY.setSpeed(2000);
  motorY.enableOutputs();

  motorZ.setEnablePin(MOTOR_Z_ENABLE_PIN);
  motorZ.setPinsInverted(false, false, true);
  motorZ.setAcceleration(87000);
  motorZ.setMaxSpeed(2000);
  motorZ.setSpeed(2000);
  motorZ.enableOutputs();

}

void runMotors() {
  while (motorX.distanceToGo() != 0 || motorY.distanceToGo() !=0 || motorZ.distanceToGo() != 0) {
    motorX.run();
    motorY.run();
    motorZ.run();
  }
  motorX.stop();
  motorY.stop();
  motorZ.stop();
}

void loop() {
  motorX.move(moveSize);
  motorY.move(moveSize);
  motorZ.move(moveSize);

  runMotors();

  motorX.move(-moveSize);
  motorY.move(-moveSize);
  motorZ.move(-moveSize);

  runMotors();

}
