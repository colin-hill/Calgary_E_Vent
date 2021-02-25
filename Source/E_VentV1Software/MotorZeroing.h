

#ifndef MotorZeroing_h
#define MotorZeroing_h

#include "elapsedMillis.h"
#include "MachineStates.h"




// ----------------------------------------------------------------------
// Functions for handling the Motor Zeroing state machine

// TODO: Add error functionality

void setupLimitSwitch(void);

void commandMoveOff(VentilatorState &state);

void moveOffSwitchWait(VentilatorState &state);

void commandHome(VentilatorState &state);

void motorHomingWait(VentilatorState &state);

void commandZero(VentilatorState &state);

void motorZeroingWait(VentilatorState &state);

void motorZero(VentilatorState &state);

void motor_zeroing_step(VentilatorState &state);



#endif