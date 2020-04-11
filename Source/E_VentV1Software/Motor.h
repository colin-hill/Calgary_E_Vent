#ifndef Motor_h
#define Motor_h

#define SERIAL_DEBUG


#include "RoboClaw.h"
#include "MachineStates.h"

//Motor Constants specific to the motor
//const float QPPR = 17700.6; //Quadrature pulses per revolution

const long int QP_TO_ZEROPOINT = 450; //Quadrature pulses from limit switch to bag edge
//const long int POSITION_TOLERANCE = 2; //Removing this for the time being
const float QP_AT_FULL_STROKE = 500; //Quadrature pulses at 100% TV that occurs from zeropoint
const float MOTOR_RETURN_FACTOR = 0.25; // Percent of nominal exalation time used to return the motor to zeropoint
const long int MOTOR_ZEROING_SPEED = 100;
const long int MOTOR_HOMING_SPEED = MOTOR_ZEROING_SPEED; //QPPS for homing
const float ZEROING_TIME = 5.0; // TODO set real time
const float HOMING_TIMEOUT = 10.0;//

const long int ACCEL = 500000;
const long int DECCEL = 500000;

const uint16_t MAX_CONTROLLER_TEMPERATURE = 60;//Set to 50C

const int MOTOR_CONTROLLER_TIMEOUT = 10000;

const float INERTIA_BUFFER = 0.02; //Seconds; The motor has inertia, we allow extra time for it too start and stop
const float HOMING_BUFFER = 1.0; //Seconds


#define MOTOR_ADDRESS 0x80 //Set on the RoboClaw controller via Basic Micro Motion Studio

void setMotorZero(RoboClaw &controller_name);

long int readPosition(RoboClaw &controller_name);

void commandStop(RoboClaw &controller_name);

//Command Functions

void commandMotorHoming(RoboClaw &controller_name);

VentilatorState commandMotorZero(RoboClaw &controller_name, VentilatorState state);

VentilatorState commandInhale(RoboClaw &controller_name, VentilatorState state);

VentilatorState commandExhale(RoboClaw &controller_name, VentilatorState state);

VentilatorState commandInhaleAbort(RoboClaw &controller_name, VentilatorState state);

VentilatorState checkMotorStatus(RoboClaw &controller_name, VentilatorState state);



//State Machine Functions

VentilatorState handle_ACMode(RoboClaw &controller_name, VentilatorState state);

VentilatorState handle_VCMode(RoboClaw &controller_name, VentilatorState state);

VentilatorState handle_MotorZeroing(RoboClaw &controller_name, VentilatorState state);

VentilatorState handle_motor(RoboClaw &controller_name, VentilatorState state);

#endif
