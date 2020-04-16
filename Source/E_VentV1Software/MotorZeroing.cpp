#define SERIAL_DEBUG //Comment this out if not debugging, used for visual confirmation of state changes
#define NO_INPUT_DEBUG //Comment this out if not debugging, used to spoof input parameters at startup when no controls are present

#include "MotorZeroing.h"

#include "alarms.h"
#include "PinAssignments.h"
#include "Motor.h"
#include "conversions.h"

#include <assert.h>

void setupLimitSwitch(void){
	pinMode(LIMIT_SWITCH_PIN, INPUT_PULLUP);
}


void commandHome(VentilatorState &state) {
    assert(state.zeroing_state == CommandHome);
#ifdef SERIAL_DEBUG
    Serial.println(F("CommandHome"));
#endif //SERIAL_DEBUG

    state.zeroing_state = MotorHomingWait;
    reset_timer(state);

    return;
}

void motorHomingWait(VentilatorState &state) {
    assert(state.zeroing_state == MotorHomingWait);

#ifdef SERIAL_DEBUG
    Serial.println(F("motorHomingWait"));
    Serial.print(F("Elaspsed time: "));
    Serial.println(elapsed_time(state));
#endif //SERIAL_DEBUG


    if (LOW == digitalRead(LIMIT_SWITCH_PIN)) {
    	state.zeroing_state = CommandZero;
    }
    else if (elapsed_time(state) > (HOMING_TIMEOUT*S_TO_MS)) {
    	//TODO: Add time out error
    }

    return;
}

void commandZero(VentilatorState &state) {
    assert(state.zeroing_state == CommandZero);

#ifdef SERIAL_DEBUG
    Serial.println(F("CommandZero"));
#endif //SERIAL_DEBUG

    state.zeroing_state = MotorZeroingWait;
    reset_timer(state);

    //TODO: Add in motor command to move inwards ZERO_POINT_TICKS at MOTOR_HOME_SPEED

    return;
}

void motorZeroingWait(VentilatorState &state) {
    assert(state.zeroing_state == MotorZeroingWait);

#ifdef SERIAL_DEBUG
    Serial.println(F("MotorZeroingWait"));
    Serial.print(F("Elaspsed time: "));
    Serial.println(elapsed_time(state));
#endif //SERIAL_DEBUG

    

    if (elapsed_time(state) > ZEROING_TIME*S_TO_MS) {

       	state.zeroing_state = MotorZero;
    }

    return;
}

void motorZero(VentilatorState &state) {
    assert(state.zeroing_state == MotorZero);

#ifdef SERIAL_DEBUG    
    Serial.println(F("motorZero"));
#endif //SERIAL_DEBUG

	//TODO: Add error if motor position is not expected

	//TODO: Add error if the motor controller is too hot

    //state.zeroing_state = CommandHome;

	state.machine_state = BreathLoopStart;

	return;
}

void motor_zeroing_step(VentilatorState &state) {

	switch(state.zeroing_state) {
	case CommandHome:
		commandHome(state);
        return;
	case MotorHomingWait:
		motorHomingWait(state);
        return;
	case CommandZero:
		commandZero(state);
        return;
	case MotorZeroingWait:
		motorZeroingWait(state);
        return;
	case MotorZero:
		motorZero(state);
        return;
	default:
		//Should not occur
		break;
	}

	return;
}
