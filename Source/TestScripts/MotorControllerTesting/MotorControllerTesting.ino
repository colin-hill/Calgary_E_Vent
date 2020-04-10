#include "RoboClaw.h"

#define MOTOR_ADDRESS 0x80

const long int ACCEL = 500000;
const long int DECCEL = 500000;

const int MOTOR_ZEROING_SPEED = 50;
const int MOTOR_HOMING_SPEED = -1*MOTOR_ZEROING_SPEED;
const long int QP_TO_ZEROPOINT = 50;

const int MOTOR_CONTROLLER_TIMEOUT = 10000;

long int pos;

RoboClaw motorController(&Serial2, MOTOR_CONTROLLER_TIMEOUT);


void setup() {
  Serial.begin(9600);

  motorController.begin(38400);

  Serial.println("Script Start");
  
}

void loop() {

  Serial.println("Loop Start");
  Serial.println("Zero --> fwd --> bwd");

  pos = readPosition(motorController);


  Serial.println("Command Homing Speed");
  commandMotorHoming(motorController);

  delay(5000);

  Serial.println("Command Stop");
  commandStop(motorController);

  delay(5000);

  Serial.println("Set Zero");
  setMotorZero(motorController);
  pos = readPosition(motorController);


  Serial.println("Go 500 ticks");
  motorController.SpeedAccelDeccelPositionM1(MOTOR_ADDRESS, ACCEL, 250, DECCEL, 500, 1);
  delay(2000);

  Serial.println("Go back 250 ticks");
  motorController.SpeedAccelDeccelPositionM1(MOTOR_ADDRESS, ACCEL, 250, DECCEL, 250, 1);
  delay(2000);


}


long int readPosition(RoboClaw &controller_name) {
  Serial.println("Encoder Position");
  long int pos = controller_name.ReadEncM1(MOTOR_ADDRESS);
  Serial.println(pos);
  return pos;
}

void commandStop(RoboClaw &controller_name) {
  controller_name.SpeedDistanceM1(MOTOR_ADDRESS, 0, 0, 1); //Stop motion
}


//Command Functions

void commandMotorHoming(RoboClaw &controller_name) {
  //command motor to move outwards
  controller_name.SpeedM1(MOTOR_ADDRESS, MOTOR_HOMING_SPEED);
}

void setMotorZero(RoboClaw &controller_name) {
  controller_name.SetEncM1(MOTOR_ADDRESS, 0);
}
