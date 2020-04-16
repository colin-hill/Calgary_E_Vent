#ifndef FailureMode_h
#define FailureMode_h

#include <Arduino.h>
#include <LiquidCrystal.h>

#include "MachineStates.h"

/* Displays mechanism failure alarm and stops all motion

   Input:
   - displays for error message
   - errors

   Output:
   - none
 */
void failure_mode(VentilatorState &state);


#endif