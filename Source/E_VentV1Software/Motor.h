#ifndef Motor_h
#define Motor_h

#define SERIAL_DEBUG


#include "RoboClaw.h"
#include "MachineStates.h"

//Motor Constants specific to the motor
const float QPPR = 17700.6; //Quadrature pulses per revolution
const long int QP_TO_ZEROPOINT = 3000; //Quadrature pulses from limit switch to bag edge
const long int POSITION_TOLERANCE = (long int) 0.01*QPPR; //Position tolerance error band for location checking, 2% currently
const float QP_AT_FULL_STROKE = 0.3*QPPR; //Quadrature pulses at 100% TV that occurs from zeropoint
const float MOTOR_RETURN_FACTOR = 0.5; // Percent of nominal exalation time used to return the motor to zeropoint
const int MOTOR_ZEROING_SPEED = 500;
const int MOTOR_HOMING_SPEED = -1*MOTOR_ZEROING_SPEED; //QPPS for homing

const int MOTOR_CONTROLLER_TIMEOUT = 10000;
const long int ACCEL = 500000;
const long int DECCEL = 500000;

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
