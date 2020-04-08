#define SERIAL_DEBUG
#include "VCMode.h"

#include "alarms.h"
#include "pressure.h"
#include "breathing.h"
#include "conversions.h"
#include "elapsedMillis.h"

#include <assert.h>


// No globals here. Want these components to be testable in isolation.

VentilatorState vcStart(VentilatorState state) {
    assert(state.vc_state == VCStart);

#ifdef SERIAL_DEBUG
    Serial.println("VCStart");
#endif //SERIAL_DEBUG

    // Reset timer and peak pressure reading.
    reset_timer(state);
    state.temp_peak_pressure = 0;

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
    // TODO: Reset timer (does this happen here as well as vcStart?)

    state.vc_state = VCInhale;
    return state;
}


VentilatorState vcInhale(VentilatorState state, const float inspiration_time) {
    assert(state.vc_state == VCInhale);

#ifdef SERIAL_DEBUG
    Serial.print("VCInhale: ");
    Serial.println(elapsed_time(state));
    Serial.print("Desired Inhale Time: ");
    Serial.println(inspiration_time);
#endif //SERIAL_DEBUG

    // TODO: Set motor position and speed
    // CB: Does this happen here, or is this what vcInhaleCommand does?

    // Monitor pressure.
    if (state.pressure > state.temp_peak_pressure) {
        state.temp_peak_pressure = state.pressure;
    }

    if(elapsed_time(state) > (inspiration_time * S_TO_MS)){
        state.vc_state = VCPeak;
        reset_timer(state);
        state.peak_pressure = state.temp_peak_pressure;
        // TODO: Check motor position********
    }

    state.errors |= check_high_pressure(state.pressure);

    // If there's high pressure, abort inhale.
    // TODO: will this state and VCInhaleAbort both raise alarm? Is that fine?
    if (state.errors & HIGH_PRESSURE_ALARM) {
        state.vc_state = VCInhaleAbort;
    }

    return state;
}


// Unused parameter warning for expiration_time due to SERIAL_DEBUG
VentilatorState vcInhaleAbort(VentilatorState state, const float expiration_time) {
    assert(state.vc_state == VCInhaleAbort);

#ifdef SERIAL_DEBUG
    Serial.print("VCInhaleAbort: ");
    Serial.println(elapsed_time(state));
    // TODO: should this really be expiration time?
    // Seems like this should be inspiration time?
    Serial.print("Desired Exhale Time: ");
    Serial.println(expiration_time);
#endif //SERIAL_DEBUG

    // TODO: Set motor velocity and desired position
    // TODO: Check if this is what is meant by resetting timer
    // TODO: Do we need to do this if vcExhale is supposed to reset the timer (but currently doesn't!)
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
    Serial.println(HOLD_TIME);
#endif //SERIAL_DEBUG
    // TODO: Hold motor in position********

    if(elapsed_time(state) > (HOLD_TIME * S_TO_MS)){
        state.vc_state = VCExhale;
        reset_timer(state);
        state.plateau_pressure = state.pressure;
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
    // TODO: Reset timer (does this happen here as well as vcStart?)

    state.vc_state = VCInhale;
    return state;
}


VentilatorState vcExhale(VentilatorState state, const float expiration_time) {
    assert(state.vc_state == VCExhale);

#ifdef SERIAL_DEBUG
    Serial.print("VCExhale: ");
    Serial.println(elapsed_time(state));
    Serial.print("Desired Exhale Time: ");
    Serial.println(expiration_time);
#endif //SERIAL_DEBUG
    // TODO: Set motor velocity and desired position

    if (elapsed_time(state) > (expiration_time * S_TO_MS)) {
        state.vc_state = VCReset;
        state.peep_pressure = state.pressure;
    }

    state.errors |= check_peep(state.pressure);
    return state;
}


// TODO: should this reset timers and stuff like acReset?
VentilatorState vcReset(VentilatorState state) {
    assert(state.vc_state == VCReset);

#ifdef SERIAL_DEBUG
    Serial.println("VCReset");
#endif //SERIAL_DEBUG

    state.machine_state = BreathLoopStart;
    return state;
}


VentilatorState vc_mode_step(VentilatorState state, const float inspiration_time, const float expiration_time) {
    switch(state.vc_state) {
    case VCStart:
        return vcStart(state);
    case VCInhaleCommand:
        return vcInhaleCommand(state);
    case VCInhale:
        return vcInhale(state, inspiration_time);
    case VCInhaleAbort:
        return vcInhaleAbort(state, expiration_time);
    case VCPeak:
        return vcPeak(state);
    case VCExhaleCommand:
        return vcExhaleCommand(state);
    case VCExhale:
        return vcExhale(state, expiration_time);
    case VCReset:
        return vcReset(state);
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
