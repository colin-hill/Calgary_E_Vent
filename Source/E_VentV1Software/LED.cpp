#include "LED.h"


void setup_LED_pins(void){

	pinMode(INHALE_LED_PIN, OUTPUT);
	pinMode(EXHALE_LED_PIN, OUTPUT);
	pinMode(VC_MODE_LED_PIN, OUTPUT);
	pinMode(AC_MODE_LED_PIN, OUTPUT);
}

void control_mode_LEDs(VentilatorState &state) {

	if (ACMode == state.machine_state) {
		digitalWrite(AC_MODE_LED_PIN, LED_ON);
		digitalWrite(VC_MODE_LED_PIN, LED_OFF);
	}
	else if (VCMode == state.machine_state) {
		digitalWrite(AC_MODE_LED_PIN, LED_OFF);
		digitalWrite(VC_MODE_LED_PIN, LED_ON);
	}
	else if (BreathLoopStart == state.machine_state) {
        //Do nothing
    }
    else {
        digitalWrite(AC_MODE_LED_PIN, LED_OFF);
        digitalWrite(VC_MODE_LED_PIN, LED_OFF);
    }
	
}

void control_inhale_exhale_LEDs(VentilatorState &state) {

	if (ACMode == state.machine_state) {
		acmode_LED(state);
	}
	else if (VCMode == state.machine_state) {
		vcmode_LED(state);

	}
    else if (BreathLoopStart == state.machine_state) {
        //Do nothing
    }
	else {
		digitalWrite(INHALE_LED_PIN, LED_OFF);
		digitalWrite(EXHALE_LED_PIN, LED_OFF);
	}
}

void acmode_LED(VentilatorState &state) {
	switch(state.ac_state) {
    case ACStart:
        digitalWrite(INHALE_LED_PIN, LED_OFF);
		digitalWrite(EXHALE_LED_PIN, LED_ON);
        return;
    case ACInhaleWait:
        digitalWrite(INHALE_LED_PIN, LED_OFF);
		digitalWrite(EXHALE_LED_PIN, LED_ON);
        return;
    case ACInhaleCommand:
        digitalWrite(INHALE_LED_PIN, LED_ON);
		digitalWrite(EXHALE_LED_PIN, LED_OFF);
        return;
    case ACInhale:
        digitalWrite(INHALE_LED_PIN, LED_ON);
		digitalWrite(EXHALE_LED_PIN, LED_OFF);
        return;
    case ACInhaleAbort:
        digitalWrite(INHALE_LED_PIN, LED_OFF);
		digitalWrite(EXHALE_LED_PIN, LED_ON);
        return;
    case ACPeak:
        digitalWrite(INHALE_LED_PIN, LED_ON);
		digitalWrite(EXHALE_LED_PIN, LED_OFF);
        return;
    case ACExhaleCommand:
        digitalWrite(INHALE_LED_PIN, LED_OFF);
		digitalWrite(EXHALE_LED_PIN, LED_ON);
        return;
    case ACExhale:
        digitalWrite(INHALE_LED_PIN, LED_OFF);
		digitalWrite(EXHALE_LED_PIN, LED_ON);
        return;
    case ACReset:
        digitalWrite(INHALE_LED_PIN, LED_OFF);
		digitalWrite(EXHALE_LED_PIN, LED_ON);
        return;
    default:
        // Should not happen
    	break;
    }
    return;	
}

void vcmode_LED(VentilatorState &state) {
	switch(state.vc_state) {
    case VCStart:
        digitalWrite(INHALE_LED_PIN, LED_OFF);
		digitalWrite(EXHALE_LED_PIN, LED_ON);
        return;
    case VCInhaleCommand:
        digitalWrite(INHALE_LED_PIN, LED_ON);
		digitalWrite(EXHALE_LED_PIN, LED_OFF);
        return;
    case VCInhale:
        digitalWrite(INHALE_LED_PIN, LED_ON);
		digitalWrite(EXHALE_LED_PIN, LED_OFF);
        return;
    case VCInhaleAbort:
        digitalWrite(INHALE_LED_PIN, LED_OFF);
		digitalWrite(EXHALE_LED_PIN, LED_ON);
        return;
    case VCPeak:
        digitalWrite(INHALE_LED_PIN, LED_ON);
		digitalWrite(EXHALE_LED_PIN, LED_OFF);
        return;
    case VCExhaleCommand:
        digitalWrite(INHALE_LED_PIN, LED_OFF);
		digitalWrite(EXHALE_LED_PIN, LED_ON);
        return;
    case VCExhale:
        digitalWrite(INHALE_LED_PIN, LED_OFF);
		digitalWrite(EXHALE_LED_PIN, LED_ON);
        return;
    case VCReset:
        digitalWrite(INHALE_LED_PIN, LED_OFF);
		digitalWrite(EXHALE_LED_PIN, LED_ON);
        return;
    default:
        // Should not happen
        break;
    }
    return;
}
