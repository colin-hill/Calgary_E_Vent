/* Functions for handling the AC Mode of the ventilator.

   AC Mode allows the patient to control the breathing of the ventilator.
 */

#ifndef ACMode_h
#define ACMode_h

#include "elapsedMillis.h"
#include "MachineStates.h"

/* States for the AC Mode state machine.

   TODO: directions to state machine diagram.
 */
enum acModeStates {
                   ACStart,
                   ACInhaleWait,
                   ACInhaleCommand,
                   ACInhale,
                   ACInhaleAbort,
                   ACPeak,
                   ACExhaleCommand,
                   ACExhale,
                   ACReset
};


// ----------------------------------------------------------------------
// Functions for handling the AC state machine.
//
// Each state is given its own function to hopefully make testing easier.
// ----------------------------------------------------------------------


/* Step the AC state machine.

   Output:
   - returns new state.
 */

acModeStates acStart(elapsedMillis &breathTimer);


// ----------------------------------------------------------------------
// Functions for handling particular states. These should NOT be called
// in the main loop, but should be documented and available for testing.
// ----------------------------------------------------------------------


acModeStates acInhaleWait(elapsedMillis &breathTimer, float &tempPeakPressure, float &pressure, float &loopThresholdPressure, uint16_t &errors);

acModeStates acInhaleCommand();

acModeStates acInhale(elapsedMillis &breathTimer, float &inspirationTime, float &tempPeakPressure, float &peakPressure, float &pressure, uint16_t &errors);


// Unused parameter warning for expirationTime due to SERIAL_DEBUG
acModeStates acInhaleAbort(elapsedMillis &breathTimer,
                           const float &expirationTime, float &pressure,
    uint16_t &errors);


acModeStates acPeak(elapsedMillis &breathTimer, float &pressure, float &plateauPressure, uint16_t &errors);

acModeStates acExhale(elapsedMillis &breathTimer, const float &expirationTime, float &pressure, float &peepPressure);


acModeStates acReset(machineStates &machineState, elapsedMillis &breathTimer, float &pressure, uint16_t &errors);


acModeStates ac_mode_step(acModeStates current_state,
                          elapsedMillis &breathTimer,
                          const float &inspirationTime,
                          const float &expirationTime, float &tempPeakPressure,
                          float &peakPressure, float &pressure,
                          float &peepPressure, float &plateauPressure,
                          float &loopThresholdPressure,
                          uint16_t &errors, machineStates &machineState);

#endif
