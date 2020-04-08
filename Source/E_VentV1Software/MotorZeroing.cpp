#define SERIAL_DEBUG //Comment this out if not debugging, used for visual confirmation of state changes
#define NO_INPUT_DEBUG //Comment this out if not debugging, used to spoof input parameters at startup when no controls are present

#include "MotorZeroing.h"

#include "alarms.h"

void setupLimitSwitch(void){
	pinMode(LIMIT_SWITCH_PIN, INPUT_PULLUP);
}


zeroingStates commandHome(elapsedMillis &homingTimer) {
#ifdef SERIAL_DEBUG
    Serial.println("CommandHome");
#endif //SERIAL_DEBUG

    zeroingStates next_state = MotorHomingWait;

    //TODO: Add in motor command to move outwards at MOTOR_HOME_SPEED

    homingTimer = 0;

    return next_state;
}

zeroingStates motorHomingWait(elapsedMillis &homingTimer, uint16_t &errors) {
#ifdef SERIAL_DEBUG
    Serial.println("motorHomingWait");
    Serial.print("Elaspsed time: ");
    Serial.println(homingTimer);
#endif //SERIAL_DEBUG

    zeroingStates next_state = MotorHomingWait;

    if (digitalRead(LIMIT_SWITCH_PIN)) {
    	next_state = CommandZero;
    }
    else if (homingTimer > HOMING_TIMEOUT) {
    	//TODO: Add error
    }

#ifdef NO_INPUT_DEBUG
    next_state = CommandZero;
#endif //Skip limit switch check

    return next_state;
}

zeroingStates commandZero(elapsedMillis &homingTimer) {
#ifdef SERIAL_DEBUG
    Serial.println("CommandZero");
#endif //SERIAL_DEBUG

    zeroingStates next_state = MotorZeroingWait;

    //TODO: Add in motor command to move inwards ZERO_POINT_TICKS at MOTOR_HOME_SPEED

    homingTimer = 0;

    return next_state;
}

zeroingStates motorZeroingWait(elapsedMillis &homingTimer) {
#ifdef SERIAL_DEBUG
    Serial.println("MotorZeroingWait");
    Serial.print("Elaspsed time: ");
    Serial.println(homingTimer);
#endif //SERIAL_DEBUG

    zeroingStates next_state = MotorZeroingWait;

    if (homingTimer > ZEROING_TIME) {

       	next_state = MotorZero;
    }

    return next_state;
}

zeroingStates motorZero(uint16_t &errors, machineStates &machineState) {
#ifdef SERIAL_DEBUG
    Serial.println("motorZero");
#endif //SERIAL_DEBUG

	//TODO: Add error if motor position is not expected

	//TODO: Add error if the motor controller is too hot

    zeroingStates next_state = CommandHome;
	machineState = BreathLoopStart;

	return next_state;
}

 zeroingStates motor_zeroing_step(zeroingStates currentState, elapsedMillis &homingTimer,uint16_t &errors, machineStates &machineState) {

	switch(currentState) {
	case CommandHome:
		return commandHome(homingTimer);
	case MotorHomingWait:
		return motorHomingWait(homingTimer, errors);
	case CommandZero:
		return commandZero(homingTimer);
	case MotorZeroingWait:
		return motorZeroingWait(homingTimer);
	case MotorZero:
		return motorZero(errors, machineState);
	default:
		//Should not occur
		break;
	}

	return currentState;
}