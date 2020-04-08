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
void failure_mode(uint16_t &errors);


#endif