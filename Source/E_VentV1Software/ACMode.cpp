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

    // Reset
    reset_timer(state);

    // TODO: CRUCIAL, double check that this makes sense. State
    // diagram mentions that peak pressure should be reset here
    // And alarms should be reset here.
    // Code does not do this, though.

    // Send motor to zero point******* (Consider watchdog timer for each state)

    state.ac_state = ACInhaleWait;
    return state;
}


VentilatorState acInhaleWait(VentilatorState state) {
    assert(state.ac_state == ACInhaleWait);

#ifdef SERIAL_DEBUG
    Serial.print("ACInhaleWait: ");
    Serial.println(elapsed_time(state));
#endif //SERIAL_DEBUG

    if (elapsed_time(state) > (AC_THRESHOLD_TIME * S_TO_MS)) {
        state.ac_state = ACInhaleCommand;

        state.errors |= APNEA_ALARM;

        reset_timer(state);
        state.temp_peak_pressure = 0;
    }
    else if(state.pressure < state.loop_threshold_pressure){
        state.ac_state = ACInhaleCommand;

        reset_timer(state);
        state.temp_peak_pressure = 0;
    }

    return state;
}


VentilatorState acInhaleCommand(VentilatorState state) {
    assert(state.ac_state == ACInhaleCommand);

#ifdef SERIAL_DEBUG
    Serial.println("ACInhaleCommand");
#endif //SERIAL_DEBUG
      
    // TODO: Set motor velocity and position********
    // TODO: set timer

    state.ac_state = ACInhale;
    return state;
}


VentilatorState acInhale(VentilatorState state, const float inspiration_time) {
    assert(state.ac_state == ACInhale);

#ifdef SERIAL_DEBUG
    Serial.print("ACInhale: ");
    Serial.println(elapsed_time(state));
    Serial.print("Desired Inhale Time: ");
    Serial.println(inspiration_time);
#endif //SERIAL_DEBUG

    // TODO: Check motor position********

    if (state.pressure > state.temp_peak_pressure) {
        // Update the peak pressure
        state.temp_peak_pressure = state.pressure;
    }

    // TODO: nervous about this else if for alarm.
    if (elapsed_time(state) > (inspiration_time * S_TO_MS)) {
        state.ac_state = ACPeak;

        reset_timer(state);
        state.peak_pressure = state.temp_peak_pressure;

        // TODO: Check that motor made it to the appropriate position********
        //Need a tolerance on this
    }
    else if (state.pressure > MAX_PRESSURE) {
        state.errors |= HIGH_PRESSURE_ALARM;
    }

    return state;
}


// Unused parameter warning for expiration_time due to SERIAL_DEBUG
// TODO: Do we really want to print exhalation time in debug here?
VentilatorState acInhaleAbort(VentilatorState state, const float expiration_time) {
    assert(state.ac_state == ACInhaleAbort);

#ifdef SERIAL_DEBUG
    Serial.print("ACInhaleAbort: ");
    Serial.println(elapsed_time(state));
    Serial.print("Desired Exhale Time: ");
    Serial.println(expiration_time);
#endif //SERIAL_DEBUG

    // TODO: Set motor velocity and desired position
    // TODO: Check if this is what is meant by resetting timer
    // TODO: Do we need to do this if acExhale is supposed to reset the timer (but currently doesn't!)
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
    Serial.println(HOLD_TIME);
#endif //SERIAL_DEBUG

    // TODO: Hold motor in position********

    // TODO: nervous about else if for alarm
    if (elapsed_time(state) > (HOLD_TIME * S_TO_MS)) { //******** how and where is hold time defined, currently hard coded
        state.ac_state = ACExhale;

        reset_timer(state);
        state.plateau_pressure = state.pressure;
    }
    else if (state.pressure > MAX_PRESSURE) {
        state.errors |= HIGH_PRESSURE_ALARM;
    }

    return state;
}

<<<<<<< HEAD
acModeStates acExhaleCommand(uint16_t &errors) {
#ifdef SERIAL_DEBUG
    Serial.println("ACExhaleCommand");
#endif //SERIAL_DEBUG

    // TODO: Set motor speed and position
    // TODO: Reset timer 

    //Need to check plateau pressure
    //errors |= check_plateau(pressure);

    return ACExhale;

}

acModeStates acExhale(elapsedMillis &breathTimer, const float &expirationTime, float &pressure, float &peepPressure) {
=======

VentilatorState acExhale(VentilatorState state, const float expiration_time) {
    assert(state.ac_state == ACExhale);

>>>>>>> master
#ifdef SERIAL_DEBUG
    Serial.print("ACExhale: ");
    Serial.println(elapsed_time(state));
    Serial.print("Desired Exhale Time: ");
    Serial.println(expiration_time);
#endif //SERIAL_DEBUG

    // TODO: Send motor to zero position********

    if (elapsed_time(state) > (expiration_time * S_TO_MS)) {
        state.ac_state      = ACReset;
        state.peep_pressure = state.pressure;
    }

    return state;
}


VentilatorState acReset(VentilatorState state) {
    assert(state.ac_state == ACReset);

#ifdef SERIAL_DEBUG
    Serial.println("ACReset");
#endif //SERIAL_DEBUG

    state.errors |= check_peep(state.pressure); // TODO: should this be peep_pressure?
    reset_timer(state);

    state.machine_state = BreathLoopStart;
    state.ac_state = ACStart;
    return state;
}


VentilatorState ac_mode_step(VentilatorState state,
                             const float inspiration_time,
                             const float expiration_time) {
    switch(state.ac_state) {
    case ACStart:
        return acStart(state);
    case ACInhaleWait:
        return acInhaleWait(state);
    case ACInhaleCommand:
        return acInhaleCommand(state);
    case ACInhale:
        return acInhale(state, inspiration_time);
    case ACInhaleAbort:
        return acInhaleAbort(state, expiration_time);
    case ACPeak:
<<<<<<< HEAD
        return acPeak(breathTimer, pressure, plateauPressure, errors);
    case ACExhaleCommand:
        return acExhaleCommand(errors);
    case ACExhale:
        return acExhale(breathTimer, expirationTime, pressure, peepPressure);
=======
        return acPeak(state);
    case ACExhale:
        // TODO: VC version has errors here, why?
        return acExhale(state, expiration_time);
>>>>>>> master
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
