#include "ACMode.h"

#include "alarms.h"
#include "pressure.h"
#include "breathing.h"
#include "conversions.h"
#include "elapsedMillis.h"
#include "Motor.h"

#include <assert.h>

// No globals here. Want these components to be testable in isolation.

void acStart(VentilatorState &state) {
    assert(state.ac_state == ACStart);

    //state.errors = 0;

#ifdef SERIAL_DEBUG
    Serial.println(F("ACStart"));
#endif //SERIAL_DEBUG

    //TODO: Reset alarms as outlined on state machine

    // Reset timer and peak pressure reading
    reset_timer(state);
    
    state.current_loop_peak_pressure = 0;

    state.ac_state = ACInhaleWait;
    return;
}


void acInhaleWait(VentilatorState &state) {
    assert(state.ac_state == ACInhaleWait);

#ifdef SERIAL_DEBUG
    Serial.print(F("ACInhaleWait: "));
    Serial.println(elapsed_time(state));
#endif //SERIAL_DEBUG

    if (elapsed_time(state) > (state.ac_threshold_time * S_TO_MS)) {
        state.ac_state = ACInhaleCommand;
   
        //state.errors |= MISSED_TRIGGER_ALARM;
    }
    else if(state.pressure < (state.peep_pressure - state.ac_threshold_pressure)){
        state.ac_state = ACInhaleCommand;
    }

    return;
}


void acInhaleCommand(VentilatorState &state) {
    assert(state.ac_state == ACInhaleCommand);

#ifdef SERIAL_DEBUG
    Serial.println(F("ACInhaleCommand"));
#endif //SERIAL_DEBUG

    reset_timer(state);

    state.ac_state = ACInhale;
    return;
}


void acInhale(VentilatorState &state, UserParameter *userParameters, RoboClaw &controller_name) {
    assert(state.ac_state == ACInhale);


    if (state.pressure > state.current_loop_peak_pressure) {
        // Update the peak pressure
        state.current_loop_peak_pressure = state.pressure;
    }

    state.errors |= check_high_pressure((PRESSURE_SAFETY_MARGIN*state.pressure), userParameters);

    if ((PRESSURE_SAFETY_MARGIN*state.pressure) > userParameters[e_HighPIPAlarm].value) {
        //Stop the motor
        commandStop(controller_name);

        state.peak_pressure = state.current_loop_peak_pressure;
        reset_timer(state);
        state.ac_state = ACInhaleAbort;
        return;
    }


    
    // TODO: nervous about this else if for alarm.
    if (elapsed_time(state) > (state.inspiration_time * S_TO_MS)) {
        state.ac_state = ACPeak;
        state.peak_pressure = state.current_loop_peak_pressure;
        state.errors |= check_pressure(state.pressure, userParameters);
        reset_timer(state);

    }

    



    return;
}


// Unused parameter warning for expiration_time due to SERIAL_DEBUG
// TODO: Do we really want to print exhalation time in debug here?
void acInhaleAbort(VentilatorState &state, UserParameter *userParameters) {
    assert(state.ac_state == ACInhaleAbort);

#ifdef SERIAL_DEBUG
    Serial.print(F("ACInhaleAbort: "));
    Serial.println(elapsed_time(state));
    Serial.print(F("Desired Exhale Time: "));
    //Serial.println(expiration_time);
#endif //SERIAL_DEBUG

    
    state.errors |= check_high_pressure((PRESSURE_SAFETY_MARGIN*state.pressure),userParameters);
    //state.peep_pressure = state.pressure;
    state.ac_state = ACExhale;

    return;
}


void acPeak(VentilatorState &state, UserParameter *userParameters) {
    assert(state.ac_state == ACPeak);

#ifdef SERIAL_DEBUG
    Serial.print(F("ACPeak: "));
    Serial.println(elapsed_time(state));
    Serial.print(F("Desired Peak Time: "));
    Serial.println(state.plateau_pause_time);
#endif //SERIAL_DEBUG

    if (elapsed_time(state) > ((state.plateau_pause_time + INERTIA_BUFFER) * S_TO_MS)) { 
        state.ac_state = ACExhaleCommand;
        reset_timer(state);
    }
    
    //Check pressure has been moved to the transition in inhale phase
    //state.errors |= check_pressure(state.pressure, userParameters);

    return;
}

void acExhaleCommand(VentilatorState &state) {
    assert(state.ac_state == ACExhaleCommand);
#ifdef SERIAL_DEBUG
    Serial.println(F("ACExhaleCommand"));
#endif //SERIAL_DEBUG

    
    state.plateau_pressure = state.pressure;
    //state.peep_pressure = state.pressure;
    state.ac_state = ACExhale;

    return;
}

void acExhale(VentilatorState &state) {
    assert(state.ac_state == ACExhale);
#ifdef SERIAL_DEBUG
    Serial.print(F("ACExhale: "));
    Serial.println(elapsed_time(state));
    Serial.print(F("Desired Exhale Time: "));
    //Serial.println(expiration_time);
#endif //SERIAL_DEBUG

    //state.peep_pressure = min(state.pressure, state.peep_pressure);

    if (elapsed_time(state) > ((state.motor_return_time + INERTIA_BUFFER) * S_TO_MS)) {//Get back faster so we can listen
        state.ac_state      = ACReset;
    }

    return;
}


void acReset(VentilatorState &state, UserParameter *userParameters) {
    assert(state.ac_state == ACReset);
#ifdef SERIAL_DEBUG
    Serial.println(F("ACReset"));
#endif //SERIAL_DEBUG

    

    //Update and check PEEP
    state.peep_pressure = state.pressure;
    state.errors |= check_peep(state.peep_pressure, userParameters); //Now checks the minimum peep_pressure for asynchronous breathing

    state.machine_state = BreathLoopStart;
    state.ac_state = ACStart;
    return;
}


void ac_mode_step(VentilatorState &state, UserParameter *userParameters, RoboClaw &controller_name) {
    switch(state.ac_state) {
    case ACStart:
        acStart(state);
        return;
    case ACInhaleWait:
        acInhaleWait(state);
        return;
    case ACInhaleCommand:
        acInhaleCommand(state);
        return;
    case ACInhale:
        acInhale(state, userParameters, controller_name);
        return;
    case ACInhaleAbort:
        acInhaleAbort(state, userParameters);
        return;
    case ACPeak:
        acPeak(state, userParameters);
        return;
    case ACExhaleCommand:
        acExhaleCommand(state);
        return;
    case ACExhale:
        acExhale(state);
        return;
    case ACReset:
        acReset(state, userParameters);
        return;
    default:
        // Should not happen
#ifdef SERIAL_DEBUG
        Serial.println(F("Invalid AC state!"));
#endif //SERIAL_DEBUG
        break;
    }

    return;
}


int acCodeAssignment(acModeStates acState) {
    return (int)acState + 1;
}
