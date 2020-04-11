

#ifndef MotorZeroing_h
#define MotorZeroing_h

#include "elapsedMillis.h"
#include "MachineStates.h"




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