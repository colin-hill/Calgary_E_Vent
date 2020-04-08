/* Functions for handling the VC Mode of the ventilator.
 */

#ifndef VCMode_h
#define VCMode_h

#include "elapsedMillis.h"
#include "MachineStates.h"


// ----------------------------------------------------------------------
// Functions for handling the VC state machine.
//
// Each state is given its own function to hopefully make testing easier.
// ----------------------------------------------------------------------

/* Step the VC state machine.

   Output:
   - returns new state.
 */
vcModeStates vc_mode_step(vcModeStates current_state, elapsedMillis &breathTimer, const float &inspirationTime, const float &expirationTime, float &tempPeakPressure, float &peakPressure, float &pressure, float &peepPressure, float &plateauPressure, uint16_t &errors, machineStates &machineState);


/* Get a debug code for the current vcModeState.

   Input:
   - state

   Output:
   - integer code (1 index of state in enum list)
 */
int vcCodeAssignment(vcModeStates vcState);


// ----------------------------------------------------------------------
// Functions for handling particular states. These should NOT be called
// in the main loop, but should be documented and available for testing.
// ----------------------------------------------------------------------

vcModeStates vcStart(elapsedMillis &breathTimer, float &tempPeakPressure);

vcModeStates vcInhaleCommand(void);

vcModeStates vcInhale(elapsedMillis &breathTimer, const float &inspirationTime, float &tempPeakPressure, float &peakPressure, float &pressure, uint16_t &errors);

vcModeStates vcInhaleAbort(elapsedMillis &breathTimer, const float &expirationTime, float &pressure, uint16_t &errors);

vcModeStates vcPeak(elapsedMillis &breathTimer, const float &inspirationTime, float &pressure, float &plateauPressure, uint16_t &errors);

vcModeStates vcExhaleCommand(void);

vcModeStates vcExhale(const elapsedMillis &breathTimer, const float &expirationTime, float &pressure, float &peepPressure, uint16_t &errors);

vcModeStates vcReset(machineStates &machineState);


#endif
