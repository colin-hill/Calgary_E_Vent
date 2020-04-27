#include "RoboClaw.h"

#define MOTOR_ADDRESS 0x80

const long int ACCEL = 500000;
const long int DECCEL = 500000;


const long int MOTOR_ZEROING_SPEED = 500;
const long int MOTOR_HOMING_SPEED = -1*MOTOR_ZEROING_SPEED;
const long int QP_TO_ZEROPOINT = 50;

const int MOTOR_CONTROLLER_TIMEOUT = 10000;

const long int pos1 = 2500;
const long int pos2 = 1250;
const long int motor_speed = 500;

long int pos;

const int alarm_pin = 11;

const int limit_switch = 18;

RoboClaw motorController(&Serial2, MOTOR_CONTROLLER_TIMEOUT);


void setup() {
  Serial.begin(9600);

  motorController.begin(38400);

  Serial.println("Script Start");

  pinMode(alarm_pin, OUTPUT);
  pinMode(limit_switch, INPUT_PULLUP);
  
}

void loop() {

  if(LOW == digitalRead(limit_switch)){
    digitalWrite(alarm_pin, HIGH);
  }
  else {
    digitalWrite(alarm_pin, LOW);
  }
  delay(50);

}

void beep(void){
  digitalWrite(alarm_pin, HIGH);
  delay(100);
  digitalWrite(alarm_pin, LOW);
  delay(100);
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
