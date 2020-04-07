#define SERIAL_DEBUG
#include "ACMode.h"

#include "alarms.h"
#include "pressure.h"
#include "breathing.h"
#include "conversions.h"
#include "elapsedMillis.h"

// No globals here. Want these components to be testable in isolation.

acModeStates acStart(elapsedMillis &breathTimer) {
#ifdef SERIAL_DEBUG
    Serial.println("ACStart");
#endif //SERIAL_DEBUG

    // Reset 
    breathTimer = 0;

    // TODO: CRUCIAL, double check that this makes sense. State
    // diagram mentions that peak pressure should be reset here
    // And alarms should be reset here.
    // Code does not do this, though.

    // Send motor to zero point******* (Consider watchdog timer for each state)
    return ACInhaleWait;
}

acModeStates acInhaleWait(elapsedMillis &breathTimer, float &tempPeakPressure, float &pressure, float &loopThresholdPressure, uint16_t &errors) {
#ifdef SERIAL_DEBUG
    Serial.print("ACInhaleWait: ");
    Serial.println(breathTimer);
#endif //SERIAL_DEBUG

    acModeStates next_state = ACInhaleWait;

    pressure = readPressureSensor();

    if (breathTimer > (AC_THRESHOLD_TIME * S_TO_MS)) {
        next_state = ACInhaleCommand;

        errors |= APNEA_ALARM;

        breathTimer = 0;
        tempPeakPressure = 0;
    }
    else if(pressure < loopThresholdPressure){
        next_state = ACInhaleCommand;

        breathTimer = 0;
        tempPeakPressure = 0;
    }

    return next_state;
}

acModeStates acInhaleCommand() {
#ifdef SERIAL_DEBUG
    Serial.println("ACInhaleCommand");
#endif //SERIAL_DEBUG
      
    // TODO: Set motor velocity and position********

    return ACInhale;
}

acModeStates acInhale(elapsedMillis &breathTimer, const float &inspirationTime, float &tempPeakPressure, float &peakPressure, float &pressure, uint16_t &errors) {
#ifdef SERIAL_DEBUG
    Serial.print("ACInhale: ");
    Serial.println(breathTimer);
    Serial.print("Desired Inhale Time: ");
    Serial.println(inspirationTime);
#endif //SERIAL_DEBUG

    acModeStates next_state = ACInhale;

    // TODO: Check motor position********

    pressure = readPressureSensor();

    if (pressure > tempPeakPressure) {
        // Update the peak pressure
        tempPeakPressure = pressure;
    }

    // TODO: nervous about this else if for alarm.
    if (breathTimer > (inspirationTime * S_TO_MS)) {
        next_state = ACPeak;

        breathTimer = 0;
        peakPressure = tempPeakPressure;

        // TODO: Check that motor made it to the appropriate position********
    }
    else if (pressure > MAX_PRESSURE) {
        errors |= HIGH_PRESSURE_ALARM;
    }

    return next_state;
}


// Unused parameter warning for expirationTime due to SERIAL_DEBUG
acModeStates acInhaleAbort(elapsedMillis &breathTimer,
                           const float &expirationTime, float &pressure,
                           uint16_t &errors) {
#ifdef SERIAL_DEBUG
    Serial.print("ACInhaleAbort: ");
    Serial.println(breathTimer);
    Serial.print("Desired Exhale Time: ");
    Serial.println(expirationTime);
#endif //SERIAL_DEBUG

    // TODO: Set motor velocity and desired position
    // TODO: Check if this is what is meant by resetting timer
    // TODO: Do we need to do this if acExhale is supposed to reset the timer (but currently doesn't!)
    breathTimer = 0;
    errors |= check_high_pressure(pressure);
    return ACExhale;
}


acModeStates acPeak(elapsedMillis &breathTimer, float &pressure, float &plateauPressure, uint16_t &errors) {
#ifdef SERIAL_DEBUG
    Serial.print("ACPeak: ");
    Serial.println(breathTimer);
    Serial.print("Desired Peak Time: ");
    Serial.println(HOLD_TIME);
#endif //SERIAL_DEBUG

    acModeStates next_state = ACPeak;

    // TODO: Hold motor in position********

    pressure = readPressureSensor();

    // TODO: nervous about else if for alarm
    if (breathTimer > (HOLD_TIME * S_TO_MS)) { //******** how and where is hold time defined, currently hard coded
        next_state = ACExhale;

        breathTimer = 0;
        plateauPressure = pressure;
    }
    else if (pressure > MAX_PRESSURE) {
        errors |= HIGH_PRESSURE_ALARM;
    }

    return next_state;
}

acModeStates acExhale(elapsedMillis &breathTimer, const float &expirationTime, float &pressure, float &peepPressure) {
#ifdef SERIAL_DEBUG
    Serial.print("ACExhale: ");
    Serial.println(breathTimer);
    Serial.print("Desired Exhale Time: ");
    Serial.println(expirationTime);
#endif //SERIAL_DEBUG

    acModeStates next_state = ACExhale;

    // TODO: Send motor to zero position********

    pressure = readPressureSensor();

    if (breathTimer > (expirationTime * S_TO_MS)) {
        next_state   = ACReset;
        peepPressure = pressure;
    }

    return next_state;
}


acModeStates acReset(machineStates &machineState, elapsedMillis &breathTimer, float &pressure, uint16_t &errors) {
#ifdef SERIAL_DEBUG
    Serial.println("ACReset");
#endif //SERIAL_DEBUG

    pressure = readPressureSensor();
    errors |= check_peep(pressure); // TODO: should this be peep_pressure?
    breathTimer = 0;

    machineState = BreathLoopStart;
    return ACStart;
}


acModeStates ac_mode_step(acModeStates current_state,
                          elapsedMillis &breathTimer,
                          const float &inspirationTime,
                          const float &expirationTime, float &tempPeakPressure,
                          float &peakPressure, float &pressure,
                          float &peepPressure, float &plateauPressure,
                          float &loopThresholdPressure,
                          uint16_t &errors, machineStates &machineState) {
    switch(current_state) {
    case ACStart:
        return acStart(breathTimer);
    case ACInhaleWait:
        return acInhaleWait(breathTimer, tempPeakPressure, pressure, loopThresholdPressure, errors);
    case ACInhaleCommand:
        return acInhaleCommand();
    case ACInhale:
        return acInhale(breathTimer, inspirationTime, tempPeakPressure, peakPressure, pressure, errors);
    case ACInhaleAbort:
        return acInhaleAbort(breathTimer, expirationTime, pressure, errors);
    case ACPeak:
        return acPeak(breathTimer, pressure, plateauPressure, errors);
    case ACExhale:
        // TODO: VC version has errors here, why?
        return acExhale(breathTimer, expirationTime, pressure, peepPressure);
    case ACReset:
        return acReset(machineState, breathTimer, pressure, errors);
    default:
        // Should not happen
#ifdef SERIAL_DEBUG
        Serial.println("Invalid AC state!");
#endif //SERIAL_DEBUG
        break;
    }

    return current_state;
}


int acCodeAssignment(acModeStates acState) {
    return (int)acState + 1;
}
