#include "ACMode.h"

#include "alarms.h"
#include "pressure.h"
#include "breathing.h"
#include "conversions.h"
#include "elapsedMillis.h"

#include <assert.h>

// No globals here. Want these components to be testable in isolation.

VentilatorState acStart(VentilatorState state) {
    assert(state.ac_state == ACStart);

#ifdef SERIAL_DEBUG
    Serial.println("ACStart");
#endif //SERIAL_DEBUG

    //TODO: Reset alarms as outlined on state machine

    // Reset timer and peak pressure reading
    reset_timer(state);
    state.current_loop_peak_pressure = 0;

    state.ac_state = ACInhaleWait;
    return state;
}


VentilatorState acInhaleWait(VentilatorState state) {
    assert(state.ac_state == ACInhaleWait);

#ifdef SERIAL_DEBUG
    Serial.print("ACInhaleWait: ");
    Serial.println(elapsed_time(state));
#endif //SERIAL_DEBUG

    if (elapsed_time(state) > (state.ac_threshold_time * S_TO_MS)) {
        state.ac_state = ACInhaleCommand;

        //Removing APNEA_ALARM for the time being based on feedback from RT
        //state.errors |= APNEA_ALARM;
    }
    else if(state.pressure < (state.peep_pressure - state.ac_threshold_pressure)){
        state.ac_state = ACInhaleCommand;
    }

    return state;
}


VentilatorState acInhaleCommand(VentilatorState state) {
    assert(state.ac_state == ACInhaleCommand);

#ifdef SERIAL_DEBUG
    Serial.println("ACInhaleCommand");
#endif //SERIAL_DEBUG

    reset_timer(state);

    state.ac_state = ACInhale;
    return state;
}


VentilatorState acInhale(VentilatorState state) {
    assert(state.ac_state == ACInhale);

#ifdef SERIAL_DEBUG
    Serial.print("ACInhale: ");
    Serial.println(elapsed_time(state));
    Serial.print("Desired Inhale Time: ");
    Serial.println(state.inspiration_time);
#endif //SERIAL_DEBUG

    if (state.pressure > state.current_loop_peak_pressure) {
        // Update the peak pressure
        state.current_loop_peak_pressure = state.pressure;
    }

    // TODO: nervous about this else if for alarm.
    if (elapsed_time(state) > (state.inspiration_time * S_TO_MS)) {
        state.ac_state = ACPeak;
        reset_timer(state);
        state.peak_pressure = state.current_loop_peak_pressure;

    }

    state.errors |= check_high_pressure(state.pressure);

    if (state.errors & HIGH_PRESSURE_ALARM) {
        state.ac_state = ACInhaleAbort;
    }

    return state;
}


// Unused parameter warning for expiration_time due to SERIAL_DEBUG
// TODO: Do we really want to print exhalation time in debug here?
VentilatorState acInhaleAbort(VentilatorState state) {
    assert(state.ac_state == ACInhaleAbort);

#ifdef SERIAL_DEBUG
    Serial.print("ACInhaleAbort: ");
    Serial.println(elapsed_time(state));
    Serial.print("Desired Exhale Time: ");
    //Serial.println(expiration_time);
#endif //SERIAL_DEBUG

    reset_timer(state);
    state.errors |= check_high_pressure(state.pressure);
    state.ac_state = ACExhale;

    return state;
}


VentilatorState acPeak(VentilatorState state) {
    assert(state.ac_state == ACPeak);

#ifdef SERIAL_DEBUG
    Serial.print("ACPeak: ");
    Serial.println(elapsed_time(state));
    Serial.print("Desired Peak Time: ");
    Serial.println(state.plateau_pause_time);
#endif //SERIAL_DEBUG

    if (elapsed_time(state) > (state.plateau_pause_time * S_TO_MS)) { 
        state.ac_state = ACExhaleCommand;
    }
    
    state.errors |= check_high_pressure(state.pressure);

    return state;
}

VentilatorState acExhaleCommand(VentilatorState state) {
    assert(state.ac_state == ACExhaleCommand);
#ifdef SERIAL_DEBUG
    Serial.println("ACExhaleCommand");
#endif //SERIAL_DEBUG

    reset_timer(state);
    state.plateau_pressure = state.pressure;
    state.ac_state = ACExhale;

    return state;
}

VentilatorState acExhale(VentilatorState state) {
    assert(state.ac_state == ACExhale);
#ifdef SERIAL_DEBUG
    Serial.print("ACExhale: ");
    Serial.println(elapsed_time(state));
    Serial.print("Desired Exhale Time: ");
    //Serial.println(expiration_time);
#endif //SERIAL_DEBUG

    if (elapsed_time(state) > (state.motor_return_time * S_TO_MS)) {
        state.ac_state      = ACReset;
    }

    return state;
}


VentilatorState acReset(VentilatorState state) {
    assert(state.ac_state == ACReset);
#ifdef SERIAL_DEBUG
    Serial.println("ACReset");
#endif //SERIAL_DEBUG

    //Update and check PEEP
    state.peep_pressure = state.pressure;
    state.errors |= check_peep(state.peep_pressure); // TODO: should this be peep_pressure?

    state.machine_state = BreathLoopStart;
    state.ac_state = ACStart;
    return state;
}


VentilatorState ac_mode_step(VentilatorState state) {
    switch(state.ac_state) {
    case ACStart:
        return acStart(state);
    case ACInhaleWait:
        return acInhaleWait(state);
    case ACInhaleCommand:
        return acInhaleCommand(state);
    case ACInhale:
        return acInhale(state);
    case ACInhaleAbort:
        return acInhaleAbort(state);
    case ACPeak:
        return acPeak(state);
    case ACExhaleCommand:
        return acExhaleCommand(state);
    case ACExhale:
        return acExhale(state);
    case ACReset:
        return acReset(state);
    default:
        // Should not happen
#ifdef SERIAL_DEBUG
        Serial.println("Invalid AC state!");
#endif //SERIAL_DEBUG
        break;
    }

    return state;
}


int acCodeAssignment(acModeStates acState) {
    return (int)acState + 1;
}
