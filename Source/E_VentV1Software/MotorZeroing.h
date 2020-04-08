

#ifndef MotorZeroing_h
#define MotorZeroing_h

#include "elapsedMillis.h"
#include "MachineStates.h"


//TODO: Add actual values/figure this out with the motor controller library
const float HOMING_TIMEOUT = 10.0;
const float ZEROING_TIME = 5.0;
const int ZERO_POINT_TICKS = 1000;
const int MOTOR_HOME_SPEED = 20; //Out of 127

const int LIMIT_SWITCH_PIN;




/* States for the Motor Zeroing Mode state machine.

   TODO: directions to state machine diagram.
 */
enum zeroingStates {
                    CommandHome,
                    MotorHomingWait,
                    CommandZero,
                    MotorZeroingWait,
                    MotorZero
};

// ----------------------------------------------------------------------
// Functions for handling the Motor Zeroing state machine

// TODO: Add error functionality

void setupLimitSwitch(void);

zeroingStates commandHome(elapsedMillis &homingTimer);

zeroingStates motorHomingWait(elapsedMillis &homingTimer);

zeroingStates commandZero(elapsedMillis &homingTimer);

zeroingStates motorZeroingWait(elapsedMillis &homingTimer);

zeroingStates motorZero(uint16_t &errors, machineStates &machineState);

zeroingStates motor_zeroing_step(zeroingStates currentState, elapsedMillis &homingTimer, uint16_t &errors, machineStates &machineState);



#endif