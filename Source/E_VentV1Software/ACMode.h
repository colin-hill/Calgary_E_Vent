/* Functions for handling the AC Mode of the ventilator.

   AC Mode allows the patient to control the breathing of the ventilator.
 */

#ifndef ACMode_h
#define ACMode_h

#include "elapsedMillis.h"
#include "MachineStates.h"


// ----------------------------------------------------------------------
// Functions for handling the AC state machine.
//
// Each state is given its own function to hopefully make testing easier.
// ----------------------------------------------------------------------


/* Step the AC state machine.

   Output:
   - returns new state.
 */
VentilatorState ac_mode_step(VentilatorState state,
    const float inspiration_time, const float expiration_time);

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

VentilatorState acStart(VentilatorState state);

VentilatorState acInhaleWait(VentilatorState state);

VentilatorState acInhaleCommand(VentilatorState state);

VentilatorState acInhale(VentilatorState state, const float inspiration_time);

// Unused parameter warning for expirationTime due to SERIAL_DEBUG
// TODO: should this be inspiration time?
VentilatorState acInhaleAbort(VentilatorState state, const float expiration_time);

VentilatorState acPeak(VentilatorState state);

VentilatorState acExhale(VentilatorState state, const float expiration_time);

VentilatorState acReset(VentilatorState state);

#endif
