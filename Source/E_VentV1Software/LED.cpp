#include "LED.h"


void setup_LED_pins(void){

	pinMode(INHALE_LED_PIN, OUTPUT);
	pinMode(EXHALE_LED_PIN, OUTPUT);
	pinMode(VC_MODE_LED_PIN, OUTPUT);
	pinMode(AC_MODE_LED_PIN, OUTPUT);
}

void control_mode_LEDs(VentilatorState &state) {

	if (ACMode == state.machine_state) {
		digitalWrite(AC_MODE_LED_PIN, HIGH);
		digitalWrite(VC_MODE_LED_PIN, LOW);
	}
	else if (VCMode == state.machine_state) {
		digitalWrite(AC_MODE_LED_PIN, LOW);
		digitalWrite(VC_MODE_LED_PIN, HIGH);
	}
	else {
		digitalWrite(AC_MODE_LED_PIN, LOW);
		digitalWrite(VC_MODE_LED_PIN, LOW);
	}
}

void control_inhale_exhale_LEDs(VentilatorState &state) {

	if (ACMode == state.machine_state) {
		acmode_LED(state);
	}
	else if (VCMode == state.machine_state) {
		vcmode_LED(state);

	}
	else {
		digitalWrite(INHALE_LED_PIN, LOW);
		digitalWrite(EXHALE_LED_PIN, LOW);
	}
}

void acmode_LED(VentilatorState &state) {
	switch(state.ac_state) {
    case ACStart:
        digitalWrite(INHALE_LED_PIN, LOW);
		digitalWrite(EXHALE_LED_PIN, HIGH);
        return;
    case ACInhaleWait:
        digitalWrite(INHALE_LED_PIN, LOW);
		digitalWrite(EXHALE_LED_PIN, HIGH);
        return;
    case ACInhaleCommand:
        digitalWrite(INHALE_LED_PIN, HIGH);
		digitalWrite(EXHALE_LED_PIN, LOW);
        return;
    case ACInhale:
        digitalWrite(INHALE_LED_PIN, HIGH);
		digitalWrite(EXHALE_LED_PIN, LOW);
        return;
    case ACInhaleAbort:
        digitalWrite(INHALE_LED_PIN, LOW);
		digitalWrite(EXHALE_LED_PIN, HIGH);
        return;
    case ACPeak:
        digitalWrite(INHALE_LED_PIN, HIGH);
		digitalWrite(EXHALE_LED_PIN, LOW);
        return;
    case ACExhaleCommand:
        digitalWrite(INHALE_LED_PIN, LOW);
		digitalWrite(EXHALE_LED_PIN, HIGH);
        return;
    case ACExhale:
        digitalWrite(INHALE_LED_PIN, LOW);
		digitalWrite(EXHALE_LED_PIN, HIGH);
        return;
    case ACReset:
        digitalWrite(INHALE_LED_PIN, LOW);
		digitalWrite(EXHALE_LED_PIN, HIGH);
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
        digitalWrite(INHALE_LED_PIN, LOW);
		digitalWrite(EXHALE_LED_PIN, HIGH);
        return;
    case VCInhaleCommand:
        digitalWrite(INHALE_LED_PIN, HIGH);
		digitalWrite(EXHALE_LED_PIN, LOW);
        return;
    case VCInhale:
        digitalWrite(INHALE_LED_PIN, HIGH);
		digitalWrite(EXHALE_LED_PIN, LOW);
        return;
    case VCInhaleAbort:
        digitalWrite(INHALE_LED_PIN, LOW);
		digitalWrite(EXHALE_LED_PIN, HIGH);
        return;
    case VCPeak:
        digitalWrite(INHALE_LED_PIN, HIGH);
		digitalWrite(EXHALE_LED_PIN, LOW);
        return;
    case VCExhaleCommand:
        digitalWrite(INHALE_LED_PIN, LOW);
		digitalWrite(EXHALE_LED_PIN, HIGH);
        return;
    case VCExhale:
        digitalWrite(INHALE_LED_PIN, LOW);
		digitalWrite(EXHALE_LED_PIN, HIGH);
        return;
    case VCReset:
        digitalWrite(INHALE_LED_PIN, LOW);
		digitalWrite(EXHALE_LED_PIN, HIGH);
        return;
    default:
        // Should not happen
        break;
    }
    return;
}
