#ifndef LCD_H
#define LCD_H


#include <Arduino.h>
#include <LiquidCrystal.h>

#include "MachineStates.h"
#include "VCMode.h"
#include "ACMode.h"

//LCD Constants
const int LCD_COLUMNS    = 20;
const int LCD_ROWS       = 4;
const int LCD_MAX_STRING = 21;


//ALARM DISPLAY FUNCTIONS---------------------------------------------------------------------------------------------------------------------------------------------

//Alarm Display
void displayNoAlarm(LiquidCrystal &displayName, float highPressure, float lowPressure, float highPEEP, float lowPEEP, float lowPlateau, const int LCD_MAX_STRING);

void displayHighPressureAlarm(LiquidCrystal &displayName, float pressureMeasurement, const int LCD_MAX_STRING);

void displayLowPressureAlarm(LiquidCrystal &displayName, float pressureMeasurement, const int LCD_MAX_STRING);

void displayHighPEEPAlarm(LiquidCrystal &displayName, float pressureMeasurement, const int LCD_MAX_STRING);

void displayLowPEEPAlarm(LiquidCrystal &displayName, float pressureMeasurement, const int LCD_MAX_STRING);

void displayDisconnectAlarm(LiquidCrystal &displayName);

void displayTemperatureAlarm(LiquidCrystal &displayName, float temperatureMeasurement, const int LCD_MAX_STRING);

void displayApneaAlarm(LiquidCrystal &displayName); //Currently will not be used

void displayDeviceFailureAlarm(LiquidCrystal &displayName);

//Alarm setpoint change
void displayHighPressureChange(LiquidCrystal &displayName, float tempHighPressure, const int LCD_MAX_STRING);

void displayLowPressureChange(LiquidCrystal &displayName, float tempLowPressure, const int LCD_MAX_STRING);

void displayHighPEEPChange(LiquidCrystal &displayName, float tempHighPEEP, const int LCD_MAX_STRING);

void displayLowPEEPChange(LiquidCrystal &displayName, float tempLowPEEP, const int LCD_MAX_STRING);

void dipslayLowPlateauChange(LiquidCrystal &displayName, float tempLowPlateau, const int LCD_MAX_STRING);

//END OF ALARM DISPLAY FUNCTIONS-------------------------------------------------------------------------------------------------------------------------------------------



//PARAMETER DISPLAY FUNCTIONS----------------------------------------------------------------------------------------------------------------------------------------------

//Parameter Display

void displayVentilationParameters(LiquidCrystal &displayName, machineStates machineState, vcModeStates vcState , acModeStates acState, float breathsPerMinute, float thresholdPressure, float tidalVolume, float inspirationTime, float inspirationPause, float measuredPIP, float measuredPlateau, const int LCD_MAX_STRING);

void displayStartupScreen(LiquidCrystal &displayName, const char softwareVersion[], const int LCD_MAX_STRING); 

void displayStartupHoldScreen(LiquidCrystal &displayName);

void displayHomingScreen(LiquidCrystal &displayName);

//Parameter setpoint change
void displayTVChange(LiquidCrystal &displayName, float tempTV, const int LCD_MAX_STRING);

void displayBPMChange(LiquidCrystal &displayName, float tempBPM, const int LCD_MAX_STRING);

void displayInspirationTimeChange(LiquidCrystal &displayName, float tempIT, const int LCD_MAX_STRING);

void displayPauseTimeChange(LiquidCrystal &displayName, float tempPauseTime, const int LCD_MAX_STRING);

void displayThresholdPressureChange(LiquidCrystal &displayName, float tempThresholdPressure, const int LCD_MAX_STRING);

//END OF PARAMETER DISPLAY FUNCTIONS--------------------------------------------------------------------------------------------------------------------------------------------


//Helper Functions

int roundAndCast(float x);

int getFirstDigitPastDecimal(float realNumber);

int getSecondDigitPastDecimal(float realNumber);

#endif
