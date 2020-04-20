#define SERIAL_DEBUG
#include "VCMode.h"

#include "alarms.h"
#include "pressure.h"
#include "breathing.h"
#include "conversions.h"
#include "elapsedMillis.h"
#include "Motor.h"

#include <assert.h>


// No globals here. Want these components to be testable in isolation.

void vcStart(VentilatorState &state) {
    assert(state.vc_state == VCStart);

    //state.errors = 0;

#ifdef SERIAL_DEBUG
    Serial.println(F("VCStart"));
#endif //SERIAL_DEBUG

    

    // Reset timer and peak pressure reading.
    reset_timer(state);
    
    state.current_loop_peak_pressure = 0;

    // Set next state.
    state.vc_state = VCInhaleCommand;

    return;
}

void vcInhaleCommand(VentilatorState &state) {
    assert(state.vc_state == VCInhaleCommand);

#ifdef SERIAL_DEBUG
    Serial.println(F("VCInhaleCommand"));
#endif //SERIAL_DEBUG

    // TODO: Set motor speed and position

    state.vc_state = VCInhale;
    return;
}


void vcInhale(VentilatorState &state, UserParameter *userParameters) {
    assert(state.vc_state == VCInhale);

#ifdef SERIAL_DEBUG
    Serial.print(F("VCInhale: "));
    Serial.println(elapsed_time(state));
    Serial.print(F("Desired Inhale Time: "));
    Serial.println(state.inspiration_time);
#endif //SERIAL_DEBUG


    // Monitor pressure.
    if (state.pressure > state.current_loop_peak_pressure) {
        state.current_loop_peak_pressure = state.pressure;
    }

    // Check time
    if(elapsed_time(state) > ((state.inspiration_time + INERTIA_BUFFER) * S_TO_MS)){
        state.vc_state = VCPeak;
        state.peak_pressure = state.current_loop_peak_pressure;
        reset_timer(state);
    }

    state.errors |= check_high_pressure(state.pressure, userParameters);

    // If there's high pressure, abort inhale.
    // TODO: will this state and VCInhaleAbort both raise alarm? Is that fine?
    // CH: Yes, I think that is fine. Basically anytime the motor is moving we want to know if the pressue is too high
     if (state.pressure > userParameters[e_HighPIPAlarm].value) {
        state.peak_pressure = state.current_loop_peak_pressure;
        state.vc_state = VCInhaleAbort;
    }

    return;
}


// Unused parameter warning for expiration_time due to SERIAL_DEBUG
void vcInhaleAbort(VentilatorState &state, UserParameter *userParameters) {
    assert(state.vc_state == VCInhaleAbort);

#ifdef SERIAL_DEBUG
    Serial.print(F("VCInhaleAbort: "));
    Serial.println(elapsed_time(state));
    // TODO: should this really be expiration time?
    // Seems like this should be inspiration time?
    Serial.print(F("Desired Exhale Time: "));
    //Serial.println(expiration_time);
#endif //SERIAL_DEBUG

    reset_timer(state);
    state.errors |= check_high_pressure(state.pressure, userParameters);
    state.vc_state = VCExhale;

    return;
}


void vcPeak(VentilatorState &state, UserParameter *userParameters) {
    assert(state.vc_state == VCPeak);

#ifdef SERIAL_DEBUG
    Serial.print(F("VCPeak: "));
    Serial.println(elapsed_time(state));
    Serial.print(F("Desired Peak Time: "));
    Serial.println(state.plateau_pause_time);
#endif //SERIAL_DEBUG
    // TODO: Hold motor in position********

    if(elapsed_time(state) > (state.plateau_pause_time * S_TO_MS)){
        state.vc_state = VCExhaleCommand;
        reset_timer(state);        
    }

    state.errors |= check_pressure(state.pressure, userParameters);
    return;
}



void vcExhaleCommand(VentilatorState &state) {
    assert(state.vc_state == VCExhaleCommand);

#ifdef SERIAL_DEBUG
    Serial.println(F("VCExhaleCommand"));
#endif //SERIAL_DEBUG

    // TODO: Set motor speed and position
    
    state.plateau_pressure = state.pressure;
    state.vc_state = VCExhale;

    return;
}


void vcExhale(VentilatorState &state) {
    assert(state.vc_state == VCExhale);

#ifdef SERIAL_DEBUG
    Serial.print(F("VCExhale: "));
    Serial.println(elapsed_time(state));
    Serial.print(F("Desired Exhale Time: "));
    Serial.println(state.expiration_time + INERTIA_BUFFER);
#endif //SERIAL_DEBUG
    // TODO: Set motor velocity and desired position

    if (elapsed_time(state) > ((state.expiration_time + INERTIA_BUFFER) * S_TO_MS)) {
        state.vc_state = VCReset;
    }

    return;
}


// TODO: should this reset timers and stuff like acReset?
void vcReset(VentilatorState &state, UserParameter *userParameters) {
    assert(state.vc_state == VCReset);

#ifdef SERIAL_DEBUG
    Serial.println(F("VCReset"));
#endif //SERIAL_DEBUG

    

    //Update and check PEEP
    state.peep_pressure = state.pressure;
    state.errors |= check_peep(state.peep_pressure, userParameters);

    state.machine_state = BreathLoopStart;
    state.vc_state = VCStart;

    return;
}


void vc_mode_step(VentilatorState &state, UserParameter *userParameters) {
    switch(state.vc_state) {
    case VCStart:
        vcStart(state);
        return;
    case VCInhaleCommand:
        vcInhaleCommand(state);
        return;
    case VCInhale:
        vcInhale(state, userParameters);
        return;
    case VCInhaleAbort:
        vcInhaleAbort(state, userParameters);
        return;
    case VCPeak:
        vcPeak(state, userParameters);
        return;
    case VCExhaleCommand:
        vcExhaleCommand(state);
        return;
    case VCExhale:
        vcExhale(state);
        return;
    case VCReset:
        vcReset(state,userParameters);
        return;
    default:
        // Should not happen
#ifdef SERIAL_DEBUG
        Serial.println(F("Invalid VC state!"));
#endif //SERIAL_DEBUG
        break;
    }

    return;
}


int vcCodeAssignment(vcModeStates vcState) {
    return (int)vcState+1;
}
