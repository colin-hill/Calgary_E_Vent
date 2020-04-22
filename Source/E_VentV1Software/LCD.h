#ifndef LCD_H
#define LCD_H


#include <Arduino.h>
#include <LiquidCrystal.h>

#include "MachineStates.h"
#include "VCMode.h"
#include "ACMode.h"
#include "PinAssignments.h"
//#include "splash.h"
#include "alarms.h"

//LCD Constants
const int LCD_COLUMNS    = 20;
const int LCD_ROWS       = 4;
const int LCD_MAX_STRING = 21;

const int VENTILATOR_LCD_ENABLE = 11;
const int VENTILATOR_LCD_RS     = 12;
const int VENTILATOR_LCD_DB4    = 5;
const int VENTILATOR_LCD_DB5    = 4;
const int VENTILATOR_LCD_DB6    = 3;
const int VENTILATOR_LCD_DB7    = 2;


//ALARM DISPLAY FUNCTIONS---------------------------------------------------------------------------------------------------------------------------------------------

//Alarm Display Functions


/** Function for alarm display when no alarms are present

   Input:
   - Current alarm setpoints
 */
void displayNoAlarm(LiquidCrystal &displayName, float highPressure, float lowPressure, float highPEEP, float lowPEEP, float lowPlateau, const int LCD_MAX_STRING);


void displayMultipleAlarms(LiquidCrystal &displayName, float  maxPIP, float minPIP, float maxPEEP, float minPEEP, float respiratoryRate, VentilatorState &state);

/** Function for display of the high PIP alarm

   Input:
   - PIP measurement
 */
void displayHighPressureAlarm(LiquidCrystal &displayName, float pressureMeasurement, const int LCD_MAX_STRING);

/** Function for display of the low PIP alarm

   Input:
   - PIP measurement
 */
void displayLowPressureAlarm(LiquidCrystal &displayName, float pressureMeasurement, const int LCD_MAX_STRING);

/** Function for display of the high PEEP alarm

   Input:
   - PEEP measurement
 */
void displayHighPEEPAlarm(LiquidCrystal &displayName, float pressureMeasurement, const int LCD_MAX_STRING);

/** Function for display of the low PEEP alarm

   Input:
   - PEEP measurement
 */
void displayLowPEEPAlarm(LiquidCrystal &displayName, float pressureMeasurement, const int LCD_MAX_STRING);

/** Function for display of a mechanical failure alarm

 */
void displayMechanicalFailureAlarm(LiquidCrystal &displayName);

/** Function for display of the high temperature alarm

   Input:
   - temperature measurement
 */
void displayTemperatureAlarm(LiquidCrystal &displayName, float temperatureMeasurement, const int LCD_MAX_STRING);

/** Function for display of an apnea alarm

 */
void displayApneaAlarm(LiquidCrystal &displayName); //Currently will not be used


/** Function for display of fatal device failure that requires a restart

 */
void displayDeviceFailureAlarm(LiquidCrystal &displayName);

//Alarm setpoint change functions

/** Function for display of the changing high PIP setpoint

   Input:
   - temporary value of the high PIP alarm setpoint
 */
void displayHighPressureChange(LiquidCrystal &displayName, float tempHighPressure, const int LCD_MAX_STRING);

/** Function for display of the changing low PIP setpoint

   Input:
   - temporary value of the low PIP alarm setpoint
 */
void displayLowPressureChange(LiquidCrystal &displayName, float tempLowPressure, const int LCD_MAX_STRING);

/** Function for display of the changing high PEEP setpoint

   Input:
   - temporary value of the high PEEP alarm setpoint
 */
void displayHighPEEPChange(LiquidCrystal &displayName, float tempHighPEEP, const int LCD_MAX_STRING);

/** Function for display of the changing low PEEP setpoint

   Input:
   - temporary value of the low PEEP alarm setpoint
 */
void displayLowPEEPChange(LiquidCrystal &displayName, float tempLowPEEP, const int LCD_MAX_STRING);

void displayHighRRChange(LiquidCrystal &displayName, float tempHighRR, const int LCD_MAX_STRING);

//END OF ALARM DISPLAY FUNCTIONS-------------------------------------------------------------------------------------------------------------------------------------------



//PARAMETER DISPLAY FUNCTIONS----------------------------------------------------------------------------------------------------------------------------------------------

//Parameter Display

/** Function for display of the ventilation setpoints and measurements

   Input:
   - ventilator states
   - ventilator setpoints
   - ventilator measurements
 */
void displayVentilationParameters(LiquidCrystal &displayName,
								  machineStates machineState, 
								  vcModeStates vcState, 
								  acModeStates acState, 
								  float breathsPerMinute, float thresholdPressure, 
								  float tidalVolume, float inspirationTime, 
								  float livePressure, float measuredPIP, 
								  float measuredPlateau, const int LCD_MAX_STRING);

/** Function for display of the software name and version at startup

   Input:
   - software version
 */
void displayStartupScreen(LiquidCrystal &displayName, const char softwareVersion[], const int LCD_MAX_STRING); 

void displayAEVStartupScreen(LiquidCrystal &displayName);

void displayStartupHoldScreen(LiquidCrystal &displayName);

/** Function for display of a homing message during motor zeroing

 */
void displayHomingScreen(LiquidCrystal &displayName);

//Parameter setpoint change functions

/** Function for display of the changing tidal volume setpoint

   Input:
   - temporary value of the tidal volume setpoint
 */
void displayTVChange(LiquidCrystal &displayName, float tempTV, const int LCD_MAX_STRING);

/** Function for display of the changing BPM setpoint

   Input:
   - temporary value of the BPM setpoint
 */
void displayBPMChange(LiquidCrystal &displayName, float tempBPM, const int LCD_MAX_STRING);

/** Function for display of the changing inspiration time setpoint

   Input:
   - temporary value of the inspiration time setpoint
 */
void displayInspirationTimeChange(LiquidCrystal &displayName, float tempIT, const int LCD_MAX_STRING);

void displayPauseTimeChange(LiquidCrystal &displayName, float tempPauseTime, const int LCD_MAX_STRING);

/** Function for display of the changing assist control threshold pressure setpoint

   Input:
   - temporary value of the threshold pressure setpoint
 */
void displayThresholdPressureChange(LiquidCrystal &displayName, float tempThresholdPressure, const int LCD_MAX_STRING);

//END OF PARAMETER DISPLAY FUNCTIONS--------------------------------------------------------------------------------------------------------------------------------------------


//Helper Functions

/** Function for conversion of a float to an int without truncation

   Input:
   - a floating point number

   Output:
   - an integer that has been rounded to the nearest whole number 
 */
int roundAndCast(float x);


/** Function that returns the first digit past the decimal of a floating point number.
	Rounding is not relevant for how this function is used for the displays.

   Input:
   - a floating point number

   Output:
   - an integer equal to the first digit past the decimal 
 */
int getFirstDigitPastDecimal(float realNumber);

/** Function that returns the second digit past the decimal of a floating point number.
	Rounding is not relevant for how this function is used for the displays.

   Input:
   - a floating point number

   Output:
   - an integer equal to the second digit past the decimal 
 */
int getSecondDigitPastDecimal(float realNumber);


//Custom Characters

const byte L1_1[]  = {
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00010,
  B00011
};

const byte L1_2[] = {
  B00000,
  B00000,
  B00100,
  B00100,
  B01110,
  B01110,
  B11111,
  B11111
};

//LINE 2
const byte L2_1[] = {
  B00000,
  B00000,
  B00000,
  B00000,
  B11100,
  B11111,
  B11111,
  B01111
};

const byte L2_2[] = {
  B00011,
  B00011,
  B00011,
  B00011,
  B10011,
  B11011,
  B11111,
  B11111
};

const byte LSS[] = {
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111
};


//LINE 3
const byte L3_1[] = {
  B01111,
  B11111,
  B01111,
  B00111,
  B00011,
  B00001,
  B00000,
  B00000
};

const byte L3_2[]  = {
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B01111
};

//LINE 4

const byte L4_1[]  = {
  B01111,
  B11000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000
};


//Virtual LED

const byte DISP_LED_OFF[] = {
  B11111,
  B10001,
  B10001,
  B10001,
  B10001,
  B10001,
  B10001,
  B11111
};

const byte DISP_LED_ON[] = {
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111
};

#endif
