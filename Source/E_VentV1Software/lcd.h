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

//TODO: add the rest of the functions


//Alarm Display Functions
void displayNoAlarm(LiquidCrystal &displayName, float highPressure, float lowPressure, float highPEEP, float lowPEEP, float lowPlateau, int lcdColumns);

void displayHighPressureAlarm(LiquidCrystal &displayName, float pressureMeasurement, int lcdColumns);

void displayLowPressureAlarm(LiquidCrystal &displayName, float pressureMeasurement, int lcdColumns);

void displayHighPEEPAlarm(LiquidCrystal &displayName, float pressureMeasurement, int lcdColumns);

void displayLowPEEPAlarm(LiquidCrystal &displayName, float pressureMeasurement, int lcdColumns);

void displayDisconnectAlarm(LiquidCrystal &displayName);

void displayTemperatureAlarm(LiquidCrystal &displayName, float temperatureMeasurement, int lcdColumns);

void displayApneaAlarm(LiquidCrystal &displayName); //Currently will not be used

void displayDeviceFailureAlarm(LiquidCrystal &displayName);


//Alarm setpoint change functions

void displayHighPressureChange(LiquidCrystal &displayName, float tempHighPressure, int lcdColumns);

void displayLowPressureChange(LiquidCrystal &displayName, float tempHighPressure, int lcdColumns);

void displayHighPEEPChange(LiquidCrystal &displayName, float tempHighPressure, int lcdColumns);

void displayLowPEEPChange(LiquidCrystal &displayName, float tempHighPressure, int lcdColumns);

void dipslayLowPlateauChange(LiquidCrystal &displayName, float tempHighPressure, int lcdColumns);


//Parameter Display Functions

//TODO




//Parameter Screen change function
void displayTVChange(LiquidCrystal &displayName, float tempTV, int lcdColumns);

void displayBPMChange(LiquidCrystal &displayName, float tempBPM, int lcdColumns);

void displayInspirationTimeChange(LiquidCrystal &displayName, float tempIT, int lcdColumns);

void displayPauseTimeChange(LiquidCrystal &displayName, float tempPauseTime, int lcdColumns);

void displayThresholdPressureChange(LiquidCrystal &displayName, float tempThresholdPressure, int lcdColumns);


//Helper Functions

int roundAndCast(float x);

char machineStateCodeAssignment(machineStates machineState);

int vcCodeAssignment(vcModeStates vcState);

int acCodeAssignment(acModeStates acState);

#endif