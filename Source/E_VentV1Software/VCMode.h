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

   Input:
   - ventilator state
   - inspiration time
   - expiration time.

   Output:
   - returns new state.
 */
VentilatorState vc_mode_step(VentilatorState state, const float inspiration_time, const float expiration_time);


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

VentilatorState vcStart(VentilatorState state);

VentilatorState vcInhaleCommand(VentilatorState state);

VentilatorState vcInhale(VentilatorState state, const float inspiration_time);

VentilatorState vcInhaleAbort(VentilatorState state, const float expiration_time);

VentilatorState vcPeak(VentilatorState state);

VentilatorState vcExhaleCommand(VentilatorState state);

VentilatorState vcExhale(VentilatorState state, const float expiration_time);

VentilatorState vcReset(VentilatorState state);

#endif
