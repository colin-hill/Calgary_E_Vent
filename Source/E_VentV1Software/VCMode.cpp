#define SERIAL_DEBUG
#include "VCMode.h"

#include "alarms.h"
#include "pressure.h"
#include "breathing.h"
#include "conversions.h"
#include "elapsedMillis.h"


// No globals here. Want these components to be testable in isolation.

vcModeStates vcStart(elapsedMillis &breathTimer, float &tempPeakPressure) {
#ifdef SERIAL_DEBUG
    Serial.println("VCStart");
#endif //SERIAL_DEBUG

    // Reset timer and peak pressure reading.
    breathTimer = 0;
    tempPeakPressure = 0;

    return VCInhaleCommand;
}


vcModeStates vcInhaleCommand(void) {
#ifdef SERIAL_DEBUG
    Serial.println("VCInhaleCommand");
#endif //SERIAL_DEBUG

    // TODO: Set motor speed and position
    // TODO: Reset timer (does this happen here as well as vcStart?)

    return VCInhale;
}

vcModeStates vcInhale(elapsedMillis &breathTimer, const float &inspirationTime,
                      float &tempPeakPressure, float &peakPressure,
                      float &pressure, uint16_t &errors) {
#ifdef SERIAL_DEBUG
    Serial.print("VCInhale: ");
    Serial.println(breathTimer);
    Serial.print("Desired Inhale Time: ");
    Serial.println(inspirationTime);
#endif //SERIAL_DEBUG

    // Default to inhaling.
    vcModeStates next_state = VCInhale;

    // TODO: Set motor position and speed
    // CB: Does this happen here, or is this what vcInhaleCommand does?

    // Monitor pressure.
    pressure = readPressureSensor();
    if (pressure > tempPeakPressure) {
        tempPeakPressure = pressure;
    }

    if(breathTimer > (inspirationTime * S_TO_MS)){
        next_state = VCPeak;
        breathTimer = 0;
        peakPressure = tempPeakPressure;
        // TODO: Check motor position********
    }

    errors |= check_high_pressure(pressure);
    //Serial.println(pressure);

    // If there's high pressure, abort inhale.
    // TODO: will this state and VCInhaleAbort raise alarm? Is that fine?
    if (errors & HIGH_PRESSURE_ALARM) {
        next_state = VCInhaleAbort;
    }

    return next_state;
}


// Unused parameter warning for expirationTime due to SERIAL_DEBUG
vcModeStates vcInhaleAbort(elapsedMillis &breathTimer,
                           const float &expirationTime, float &pressure,
                           uint16_t &errors) {
#ifdef SERIAL_DEBUG
    Serial.print("VCInhaleAbort: ");
    Serial.println(breathTimer);
    Serial.print("Desired Exhale Time: ");
    Serial.println(expirationTime);
#endif //SERIAL_DEBUG

    // TODO: Set motor velocity and desired position
    // TODO: Check if this is what is meant by resetting timer
    // TODO: Do we need to do this if vcExhale is supposed to reset the timer (but currently doesn't!)
    breathTimer = 0;
    errors |= check_high_pressure(pressure);
    return VCExhale;
}


// Unused parameter warning is due to inspirationTime only being used for debug information.
// The debug information should be displayingthe hold tim, this has been altered
vcModeStates vcPeak(elapsedMillis &breathTimer, const float &inspirationTime,
                    float &pressure, float &plateauPressure, float &loopPlateauPause,
                    uint16_t &errors) {
#ifdef SERIAL_DEBUG
    Serial.print("VCPeak: ");
    Serial.println(breathTimer);
    Serial.print("Desired Peak Time: ");
    Serial.println(loopPlateauPause);
#endif //SERIAL_DEBUG

    vcModeStates next_state = VCPeak;

    // TODO: Hold motor in position********

    pressure = readPressureSensor();
    if(breathTimer > (loopPlateauPause * S_TO_MS)){
        next_state  = VCExhale;
        breathTimer = 0;
        plateauPressure = pressure;
    }

    errors |= check_high_pressure(pressure);
    return next_state;
}

vcModeStates vcExhaleCommand(uint16_t &errors) {

#ifdef SERIAL_DEBUG
    Serial.println("VCExhaleCommand");
#endif //SERIAL_DEBUG

    // TODO: Set motor speed and position
    // TODO: Reset timer (does this happen here as well as vcStart?)

    //Need to check plateau pressure
    //errors |= check_plateau(pressure);

    return VCExhale;
}

vcModeStates vcExhale(const elapsedMillis &breathTimer,
                      const float &expirationTime, float &pressure,
                      float &peepPressure) {
#ifdef SERIAL_DEBUG
    Serial.print("VCExhale: ");
    Serial.println(breathTimer);
    Serial.print("Desired Exhale Time: ");
    Serial.println(expirationTime);
#endif //SERIAL_DEBUG

    vcModeStates next_state = VCExhale;

    // TODO: Set motor velocity and desired position

    pressure = readPressureSensor();
    if (breathTimer > (expirationTime * S_TO_MS)) {
        next_state = VCReset;
        peepPressure = pressure;
    }

    
    return next_state;
}


// TODO: should this reset timers and stuff like acReset?
vcModeStates vcReset(machineStates &machineState, float &peepPressure, uint16_t &errors) {
#ifdef SERIAL_DEBUG
    Serial.println("VCReset");
#endif //SERIAL_DEBUG

    errors |= check_peep(peepPressure);

    machineState = BreathLoopStart;
    return VCStart;
}


vcModeStates vc_mode_step(vcModeStates current_state,
                          elapsedMillis &breathTimer,
                          const float &inspirationTime,
                          const float &expirationTime, float &tempPeakPressure,
                          float &peakPressure, float &pressure,
                          float &peepPressure, float &plateauPressure, float &loopPlateauPause,
                          uint16_t &errors, machineStates &machineState) {
    switch(current_state) {
    case VCStart:
        return vcStart(breathTimer, tempPeakPressure);
    case VCInhaleCommand:
        return vcInhaleCommand();
    case VCInhale:
        return vcInhale(breathTimer, inspirationTime, tempPeakPressure, peakPressure, pressure, errors);
    case VCInhaleAbort:
        return vcInhaleAbort(breathTimer, expirationTime, pressure, errors);
    case VCPeak:
        return vcPeak(breathTimer, inspirationTime, pressure, plateauPressure, loopPlateauPause, errors);
    case VCExhaleCommand:
        return vcExhaleCommand(errors); 
    case VCExhale:
        return vcExhale(breathTimer, expirationTime, pressure, peepPressure);
    case VCReset:
        return vcReset(machineState, peepPressure, errors);
    default:
        // Should not happen
#ifdef SERIAL_DEBUG
        Serial.println("Invalid VC state!");
#endif //SERIAL_DEBUG
        break;
    }

    return current_state;
}


int vcCodeAssignment(vcModeStates vcState) {
    return (int)vcState+1;
}
