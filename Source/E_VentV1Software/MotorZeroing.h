

#ifndef MotorZeroing_h
#define MotorZeroing_h

#include "elapsedMillis.h"
#include "MachineStates.h"


//TODO: Add actual values/figure this out with the motor controller library
const float HOMING_TIMEOUT = 10.0;
const float ZEROING_TIME = 5.0;
const int ZERO_POINT_TICKS = 1000;
const int MOTOR_HOME_SPEED = 20; //Out of 127


// ----------------------------------------------------------------------
// Functions for handling the Motor Zeroing state machine

// TODO: Add error functionality

void setupLimitSwitch(void);

VentilatorState commandHome(VentilatorState state);

VentilatorState motorHomingWait(VentilatorState state);

VentilatorState commandZero(VentilatorState state);

VentilatorState motorZeroingWait(VentilatorState state);

VentilatorState motorZero(VentilatorState state);

VentilatorState motor_zeroing_step(VentilatorState state);



#endif