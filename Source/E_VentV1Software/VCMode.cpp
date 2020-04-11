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

VentilatorState vcStart(VentilatorState state) {
    assert(state.vc_state == VCStart);

    //state.errors = 0;

#ifdef SERIAL_DEBUG
    Serial.println("VCStart");
#endif //SERIAL_DEBUG

    //TODO: Reset alarms as outlined on state machine

    // Reset timer and peak pressure reading.
    reset_timer(state);
    state.current_loop_peak_pressure = 0;

    // Set next state.
    state.vc_state = VCInhaleCommand;

    return state;
}


VentilatorState vcInhaleCommand(VentilatorState state) {
    assert(state.vc_state == VCInhaleCommand);

#ifdef SERIAL_DEBUG
    Serial.println("VCInhaleCommand");
#endif //SERIAL_DEBUG

    // TODO: Set motor speed and position

    state.vc_state = VCInhale;
    return state;
}


VentilatorState vcInhale(VentilatorState state) {
    assert(state.vc_state == VCInhale);

#ifdef SERIAL_DEBUG
    Serial.print("VCInhale: ");
    Serial.println(elapsed_time(state));
    Serial.print("Desired Inhale Time: ");
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

    state.errors |= check_high_pressure(state.pressure);

    // If there's high pressure, abort inhale.
    // TODO: will this state and VCInhaleAbort both raise alarm? Is that fine?
    // CH: Yes, I think that is fine. Basically anytime the motor is moving we want to know if the pressue is too high
    if (state.errors & HIGH_PRESSURE_ALARM) {
        state.vc_state = VCInhaleAbort;
    }

    return state;
}


// Unused parameter warning for expiration_time due to SERIAL_DEBUG
VentilatorState vcInhaleAbort(VentilatorState state) {
    assert(state.vc_state == VCInhaleAbort);

#ifdef SERIAL_DEBUG
    Serial.print("VCInhaleAbort: ");
    Serial.println(elapsed_time(state));
    // TODO: should this really be expiration time?
    // Seems like this should be inspiration time?
    Serial.print("Desired Exhale Time: ");
    //Serial.println(expiration_time);
#endif //SERIAL_DEBUG

    reset_timer(state);
    state.errors |= check_high_pressure(state.pressure);
    state.vc_state = VCExhale;

    return state;
}


VentilatorState vcPeak(VentilatorState state) {
    assert(state.vc_state == VCPeak);

#ifdef SERIAL_DEBUG
    Serial.print("VCPeak: ");
    Serial.println(elapsed_time(state));
    Serial.print("Desired Peak Time: ");
    Serial.println(state.plateau_pause_time);
#endif //SERIAL_DEBUG
    // TODO: Hold motor in position********

    if(elapsed_time(state) > (state.plateau_pause_time * S_TO_MS)){
        state.vc_state = VCExhaleCommand;        
    }

    state.errors |= check_high_pressure(state.pressure);
    return state;
}



VentilatorState vcExhaleCommand(VentilatorState state) {
    assert(state.vc_state == VCExhaleCommand);

#ifdef SERIAL_DEBUG
    Serial.println("VCExhaleCommand");
#endif //SERIAL_DEBUG

    // TODO: Set motor speed and position
    reset_timer(state);
    state.plateau_pressure = state.pressure;
    state.vc_state = VCExhale;

    return state;
}


VentilatorState vcExhale(VentilatorState state) {
    assert(state.vc_state == VCExhale);

#ifdef SERIAL_DEBUG
    Serial.print("VCExhale: ");
    Serial.println(elapsed_time(state));
    Serial.print("Desired Exhale Time: ");
    Serial.println(state.motor_return_time);
#endif //SERIAL_DEBUG
    // TODO: Set motor velocity and desired position

    if (elapsed_time(state) > ((state.expiration_time + INERTIA_BUFFER) * S_TO_MS)) {
        state.vc_state = VCReset;
    }

    return state;
}


// TODO: should this reset timers and stuff like acReset?
VentilatorState vcReset(VentilatorState state, UserParameter *userParameters) {
    assert(state.vc_state == VCReset);

#ifdef SERIAL_DEBUG
    Serial.println("VCReset");
#endif //SERIAL_DEBUG

    //Update and check PEEP
    state.peep_pressure = state.pressure;
    state.errors |= check_peep(state.peep_pressure, userParameters);

    state.machine_state = BreathLoopStart;
    state.vc_state = VCStart;

    return state;
}


VentilatorState vc_mode_step(VentilatorState state, UserParameter *userParameters) {
    switch(state.vc_state) {
    case VCStart:
        return vcStart(state);
    case VCInhaleCommand:
        return vcInhaleCommand(state);
    case VCInhale:
        return vcInhale(state);
    case VCInhaleAbort:
        return vcInhaleAbort(state);
    case VCPeak:
        return vcPeak(state);
    case VCExhaleCommand:
        return vcExhaleCommand(state);
    case VCExhale:
        return vcExhale(state);
    case VCReset:
        return vcReset(state,userParameters);
    default:
        // Should not happen
#ifdef SERIAL_DEBUG
        Serial.println("Invalid VC state!");
#endif //SERIAL_DEBUG
        break;
    }

    return state;
}


int vcCodeAssignment(vcModeStates vcState) {
    return (int)vcState+1;
}
