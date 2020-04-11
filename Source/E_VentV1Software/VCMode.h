/* Functions for handling the VC Mode of the ventilator.
 */

#ifndef VCMode_h
#define VCMode_h

#include "elapsedMillis.h"
#include "MachineStates.h"
#include "UserParameter.h"


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
VentilatorState vc_mode_step(VentilatorState state, UserParameter *userParameters);


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

// TODO: These need to be documented with pre and postconditions.
// These pre/post conditions currently exist in a spreadsheet.
VentilatorState vcStart(VentilatorState state);

VentilatorState vcInhaleCommand(VentilatorState state);

VentilatorState vcInhale(VentilatorState state,UserParameter *userParameters);

VentilatorState vcInhaleAbort(VentilatorState state, UserParameter *userParameters);

VentilatorState vcPeak(VentilatorState state, UserParameter *userParameters);

VentilatorState vcExhaleCommand(VentilatorState state);

VentilatorState vcExhale(VentilatorState state);

VentilatorState vcReset(VentilatorState state);

#endif
