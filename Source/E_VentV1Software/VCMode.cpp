#include "VCMode.h"

#include "alarms.h"
#include "pressure.h"
#include "breathing.h"
#include "elapsedMillis.h"

// No globals here. Want these components to be testable in isolation.

vcModeStates vcStart(elapsedMillis &breathTimer, float &tempPeakPressure) {
#ifdef SERIAL_DEBUG
    Serial.print("VCStart");
#endif //SERIAL_DEBUG

    // Reset timer and peak pressure reading.
    breathTimer = 0;
    tempPeakPressure = 0;

    return VCInhaleCommand;
}

vcModeStates vcInhaleCommand(void) {
#ifdef SERIAL_DEBUG
    Serial.print("VCInhaleCommand");
#endif //SERIAL_DEBUG

    // TODO: Set motor speed and position
    // TODO: Reset timer

    return VCInhale;
}

vcModeStates vcInhale(elapsedMillis &breathTimer, const float &inspirationTime, float &tempPeakPressure, float &pressure, float &peakPressure, uint16_t &errors) {
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

    if(breathTimer > inspirationTime*1000){
        next_state = VCPeak;
        breathTimer = 0;
        peakPressure = tempPeakPressure;
        // TODO: Check motor position********
    }

    errors |= check_high_pressure(pressure);
    return next_state;
}

vcModeStates vcPeak(elapsedMillis &breathTimer, const float &inspirationTime, float &pressure, float &plateauPressure, uint16_t &errors) {
#ifdef SERIAL_DEBUG
    Serial.print("VCPeak: ");
    Serial.println(breathTimer);
    Serial.print("Desired Peak Time: ");
    Serial.println(inspirationTime);
#endif //SERIAL_DEBUG

    vcModeStates next_state = VCPeak;

    // TODO: Hold motor in position********

    pressure = readPressureSensor();
    if(breathTimer > HOLD_TIME*1000){
        next_state  = VCExhale;
        breathTimer = 0;
        plateauPressure = pressure;
    }

    errors |= check_high_pressure(pressure);
    return next_state;
}

vcModeStates vcExhale(const elapsedMillis &breathTimer, const float &expirationTime, float &pressure, float &peepPressure, uint16_t &errors) {
#ifdef SERIAL_DEBUG
    Serial.print("VCExhale: ");
    Serial.println(breathTimer);
    Serial.print("Desired Exhale Time: ");
    Serial.println(expirationTime);
#endif //SERIAL_DEBUG

    vcModeStates next_state = VCExhale;

    // TODO: Set motor vlocity and desired position

    pressure = readPressureSensor();
    if (breathTimer > expirationTime) {
        next_state = VCReset;
        peepPressure = pressure;
    }

    errors |= check_peep(pressure);
    return next_state;
}

// TODO: GRRRR
vcModeStates vcReset() {
#ifdef SERIAL_DEBUG
    Serial.print("VCReset");
#endif //SERIAL_DEBUG

    // machineState = BreathLoopStart;
    // vcModeState = VCStart;
}


void run_vc_mode(vcModeStates current_state) {
    // TODO: loop
    switch(current_state) {
    case VCStart:
        break;
    case VCInhaleCommand:
        break;
    case VCInhale:
        break;
    case VCInhaleAbort:
        break;
    case VCPeak:
        break;
    case VCExhale:
        break;
    case VCReset:
        break;
    default:
        // Should not happen
#ifdef SERIAL_DEBUG
        Serial.println("Invalid VC state!");
#endif //SERIAL_DEBUG
        break;
    }
}
