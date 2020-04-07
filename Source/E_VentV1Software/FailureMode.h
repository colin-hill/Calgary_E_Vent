#ifndef FailureMode_h
#define FailureMode_h

#include <Arduino.h>
#include <LiquidCrystal.h>

/* Displays mechanism failure alarm and stops all motion

   Input:
   - displays for error message
   - errors

   Output:
   - none
 */
void failure_mode(LiquidCrystal &displayName, uint16_t &errors, float peakPressure, float peepPressure, float controllerTemperature);


#endif