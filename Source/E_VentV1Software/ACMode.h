/* Functions for handling the AC Mode of the ventilator.

   AC Mode allows the patient to control the breathing of the ventilator.
 */

#ifndef ACMode_h
#define ACMode_h

#include "elapsedMillis.h"
#include "MachineStates.h"
#include "UserParameter.h"
#define SERIAL_DEBUG

// ----------------------------------------------------------------------
// Functions for handling the AC state machine.
//
// Each state is given its own function to hopefully make testing easier.
// ----------------------------------------------------------------------


/* Step the AC state machine.

   Output:
   - returns new state.
 */

void ac_mode_step(VentilatorState &state, UserParameter *userParameters);

/* Get a debug code for the current acModeState.

   Input:
   - state

   Output:
   - integer code (1 index of state in enum list)
 */
int acCodeAssignment(acModeStates acState);


// ----------------------------------------------------------------------
// Functions for handling particular states. These should NOT be called
// in the main loop, but should be documented and available for testing.
// ----------------------------------------------------------------------


// TODO: THESE MUST BE DOCUMENTED!

void acStart(VentilatorState &state);

void acInhaleWait(VentilatorState &state);



void acInhaleCommand(VentilatorState &state);

void acInhale(VentilatorState &state, UserParameter *userParameters);

// Unused parameter warning for expirationTime due to SERIAL_DEBUG
// TODO: should this be inspiration time?
void acInhaleAbort(VentilatorState &state, UserParameter *userParameters);

void acPeak(VentilatorState &state, UserParameter *userParameters);

void acExhaleCommand(VentilatorState &state);

void acExhale(VentilatorState &state);

void acReset(VentilatorState &state, UserParameter *userParameters);
#endif
