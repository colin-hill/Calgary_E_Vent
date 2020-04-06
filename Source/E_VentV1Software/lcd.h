#ifndef LCD_H
#define LCD_H


#include <Arduino.h>
#include <LiquidCrystal.h>

#include "MachineStates.h"
#include "VCMode.h"
#include "ACMode.h"

//LCD Constants
const int LCD_COLUMNS = 20;
const int LCD_ROWS = 4;


//ALARM DISPLAY FUNCTIONS---------------------------------------------------------------------------------------------------------------------------------------------

//Alarm Display
void displayNoAlarm(LiquidCrystal &displayName, float highPressure, float lowPressure, float highPEEP, float lowPEEP, float lowPlateau, const int LCD_COLUMNS);

void displayHighPressureAlarm(LiquidCrystal &displayName, float pressureMeasurement, const int LCD_COLUMNS);

void displayLowPressureAlarm(LiquidCrystal &displayName, float pressureMeasurement, const int LCD_COLUMNS);

void displayHighPEEPAlarm(LiquidCrystal &displayName, float pressureMeasurement, const int LCD_COLUMNS);

void displayLowPEEPAlarm(LiquidCrystal &displayName, float pressureMeasurement, const int LCD_COLUMNS);

void displayDisconnectAlarm(LiquidCrystal &displayName);

void displayTemperatureAlarm(LiquidCrystal &displayName, float temperatureMeasurement, const int LCD_COLUMNS);

void displayApneaAlarm(LiquidCrystal &displayName); //Currently will not be used

void displayDeviceFailureAlarm(LiquidCrystal &displayName);

//Alarm setpoint change 
void displayHighPressureChange(LiquidCrystal &displayName, float tempHighPressure, const int LCD_COLUMNS);

void displayLowPressureChange(LiquidCrystal &displayName, float tempHighPressure, const int LCD_COLUMNS);

void displayHighPEEPChange(LiquidCrystal &displayName, float tempHighPressure, const int LCD_COLUMNS);

void displayLowPEEPChange(LiquidCrystal &displayName, float tempHighPressure, const int LCD_COLUMNS);

void dipslayLowPlateauChange(LiquidCrystal &displayName, float tempHighPressure, const int LCD_COLUMNS);

//END OF ALARM DISPLAY FUNCTIONS-------------------------------------------------------------------------------------------------------------------------------------------



//PARAMETER DISPLAY FUNCTIONS----------------------------------------------------------------------------------------------------------------------------------------------

//Parameter Display

void displayVentilationParameters(LiquidCrystal &displayName, machineStates machineState, vcModeStates vcState , acModeStates acState, float breathsPerMinute, float thresholdPressure, float tidalVolume, float inspirationTime, float inspirationPause, float measuredPIP, float measuredPlateau, const int LCD_COLUMNS);

void displayStartupScreen(LiquidCrystal &displayName, const char softwareVersion[], const int LCD_COLUMNS); //TODO fix warning

void displayHomingScreen(LiquidCrystal &displayName);

//Parameter setpoint change
void displayTVChange(LiquidCrystal &displayName, float tempTV, const int LCD_COLUMNS);

void displayBPMChange(LiquidCrystal &displayName, float tempBPM, const int LCD_COLUMNS);

void displayInspirationTimeChange(LiquidCrystal &displayName, float tempIT, const int LCD_COLUMNS);

void displayPauseTimeChange(LiquidCrystal &displayName, float tempPauseTime, const int LCD_COLUMNS);

void displayThresholdPressureChange(LiquidCrystal &displayName, float tempThresholdPressure, const int LCD_COLUMNS);

//END OF PARAMETER DISPLAY FUNCTIONS--------------------------------------------------------------------------------------------------------------------------------------------


//Helper Functions

int roundAndCast(float x);

char machineStateCodeAssignment(machineStates machineState);

int vcCodeAssignment(vcModeStates vcState);

int acCodeAssignment(acModeStates acState);

int getFirstDigitPastDecimal(float realNumber);

int getSecondDigitPastDecimal(float realNumber);

#endif