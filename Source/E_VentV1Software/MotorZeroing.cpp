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


VentilatorState commandHome(VentilatorState state) {
    assert(state.zeroing_state == CommandHome);
#ifdef SERIAL_DEBUG
    Serial.println("CommandHome");
#endif //SERIAL_DEBUG

    state.zeroing_state = MotorHomingWait;
    reset_timer(state);

    return state;
}

VentilatorState motorHomingWait(VentilatorState state) {
    assert(state.zeroing_state == MotorHomingWait);

#ifdef SERIAL_DEBUG
    Serial.println("motorHomingWait");
    Serial.print("Elaspsed time: ");
    Serial.println(elapsed_time(state));
#endif //SERIAL_DEBUG


    if (LOW == digitalRead(LIMIT_SWITCH_PIN)) {
    	state.zeroing_state = CommandZero;
    }
    else if (elapsed_time(state) > (HOMING_TIMEOUT*S_TO_MS)) {
    	//TODO: Add time out error
    }

    return state;
}

VentilatorState commandZero(VentilatorState state) {
    assert(state.zeroing_state == CommandZero);

#ifdef SERIAL_DEBUG
    Serial.println("CommandZero");
#endif //SERIAL_DEBUG

    state.zeroing_state = MotorZeroingWait;
    reset_timer(state);

    //TODO: Add in motor command to move inwards ZERO_POINT_TICKS at MOTOR_HOME_SPEED

    return state;
}

VentilatorState motorZeroingWait(VentilatorState state) {
    assert(state.zeroing_state == MotorZeroingWait);

#ifdef SERIAL_DEBUG
    Serial.println("MotorZeroingWait");
    Serial.print("Elaspsed time: ");
    Serial.println(elapsed_time(state));
#endif //SERIAL_DEBUG

    

    if (elapsed_time(state) > ZEROING_TIME*S_TO_MS) {

       	state.zeroing_state = MotorZero;
    }

    return state;
}

VentilatorState motorZero(VentilatorState state) {
    assert(state.zeroing_state == MotorZero);

#ifdef SERIAL_DEBUG    
    Serial.println("motorZero");
#endif //SERIAL_DEBUG

	//TODO: Add error if motor position is not expected

	//TODO: Add error if the motor controller is too hot

    //state.zeroing_state = CommandHome;

	state.machine_state = BreathLoopStart;

	return state;
}

VentilatorState motor_zeroing_step(VentilatorState state) {

	switch(state.zeroing_state) {
	case CommandHome:
		return commandHome(state);
	case MotorHomingWait:
		return motorHomingWait(state);
	case CommandZero:
		return commandZero(state);
	case MotorZeroingWait:
		return motorZeroingWait(state);
	case MotorZero:
		return motorZero(state);
	default:
		//Should not occur
		break;
	}

	return state;
}
