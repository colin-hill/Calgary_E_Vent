#include "LCD.h"

//Alarm Display Functions

void displayNoAlarm(LiquidCrystal &displayName, float highPressure, float lowPressure, float highPEEP, float lowPEEP, float lowPlateau, const int LCD_MAX_STRING) {
        char alarmStr[LCD_MAX_STRING];
	
	int displayHighPressure = roundAndCast(highPressure);
	int displayLowPressure = roundAndCast(lowPressure);
	int displayHighPEEP = roundAndCast(highPEEP);
	int displayLowPEEP = roundAndCast(lowPEEP);
	int displayLowPlateau = roundAndCast(lowPlateau);

	displayName.clear();

        // First line
	displayName.write("NO ALARM  SETPOINTS:");

        // Second line
	snprintf(alarmStr, LCD_MAX_STRING, "PIP=%11d-%2dCM", displayLowPressure, displayHighPressure);
	displayName.setCursor(0,1);
	displayName.write(alarmStr);

        // Third line
	snprintf(alarmStr, LCD_MAX_STRING, "PEEP=%10d-%2dCM", displayLowPEEP, displayHighPEEP);
	displayName.setCursor(0,2);
	displayName.write(alarmStr);

        // Fourth line
	displayName.setCursor(0,3);
	snprintf(alarmStr, LCD_MAX_STRING, "PLATEAU MIN=%6dCM", displayLowPlateau);
	displayName.write(alarmStr);
}


void displayHighPressureAlarm(LiquidCrystal &displayName, float pressureMeasurement, const int LCD_MAX_STRING) {
	int displayPressure = roundAndCast(pressureMeasurement);

	char alarmDispL4[LCD_MAX_STRING];
	snprintf(alarmDispL4, LCD_MAX_STRING, "PRESSURE=%3d CM", displayPressure);

	displayName.clear();

        // First line
	displayName.write("ALARM CONDITION:");

        // Third line
	displayName.setCursor(0,2);
	displayName.write("HIGH INSPIRATION");

        // Fourth line
	displayName.setCursor(0,3);
	displayName.write(alarmDispL4);
}

void displayLowPressureAlarm(LiquidCrystal &displayName, float pressureMeasurement, const int LCD_MAX_STRING) {
	
	int displayPressure = roundAndCast(pressureMeasurement);

	const char alarmDispL1[] = "ALARM CONDITION:";
	const char alarmDispL3[] = "LOW INSPIRATION";
	char alarmDispL4[LCD_MAX_STRING];
	snprintf(alarmDispL4, LCD_MAX_STRING, "PRESSURE=%3d CM", displayPressure);

	displayName.clear();
	displayName.write(alarmDispL1);
	displayName.setCursor(0,2);
	displayName.write(alarmDispL3);
	displayName.setCursor(0,3);
	displayName.write(alarmDispL4);

}

void displayHighPEEPAlarm(LiquidCrystal &displayName, float pressureMeasurement, const int LCD_MAX_STRING) {
	
	int displayPressure = roundAndCast(pressureMeasurement);

	const char alarmDispL1[] = "ALARM CONDITION:";
	const char alarmDispL3[] = "HIGH PEEP";
	char alarmDispL4[LCD_MAX_STRING];
	snprintf(alarmDispL4, LCD_MAX_STRING, "PRESSURE=%3d CM", displayPressure);

	displayName.clear();
	displayName.write(alarmDispL1);
	displayName.setCursor(0,2);
	displayName.write(alarmDispL3);
	displayName.setCursor(0,3);
	displayName.write(alarmDispL4);

}

void displayLowPEEPAlarm(LiquidCrystal &displayName, float pressureMeasurement, const int LCD_MAX_STRING) {
	
	int displayPressure = roundAndCast(pressureMeasurement);

	const char alarmDispL1[] = "ALARM CONDITION:";
	const char alarmDispL3[] = "LOW PEEP";
	char alarmDispL4[LCD_MAX_STRING];
	snprintf(alarmDispL4, LCD_MAX_STRING, "PRESSURE=%3d CM", displayPressure);

	displayName.clear();
	displayName.write(alarmDispL1);
	displayName.setCursor(0,2);
	displayName.write(alarmDispL3);
	displayName.setCursor(0,3);
	displayName.write(alarmDispL4);

}


void displayDisconnectAlarm(LiquidCrystal &displayName) {
	
	const char alarmDispL1[] = "ALARM CONDITION:";
	const char alarmDispL2[] = "POSSIBLE DISCONNECT";
	const char alarmDispL3[] = "CHECK O2 AND AIR";
	const char alarmDispL4[] = "CONNECTIONS";

	displayName.clear();
	displayName.write(alarmDispL1);
	displayName.setCursor(0,1);
	displayName.write(alarmDispL2);
	displayName.setCursor(0,2);
	displayName.write(alarmDispL3);
	displayName.setCursor(0,3);
	displayName.write(alarmDispL4);

}


void displayTemperatureAlarm(LiquidCrystal &displayName, float temperatureMeasurement, int const LCD_MAX_STRING) {
	
	int displayTemperature = roundAndCast(temperatureMeasurement);

	const char alarmDispL1[] = "ALARM CONDITION:";
	const char alarmDispL3[] = "HIGH CONTROLLER TEMP";
	char alarmDispL4[LCD_MAX_STRING];
	snprintf(alarmDispL4, LCD_MAX_STRING, "TEMPERATURE=%3d C", displayTemperature);

	displayName.clear();
	displayName.write(alarmDispL1);
	displayName.setCursor(0,2);
	displayName.write(alarmDispL3);
	displayName.setCursor(0,3);
	displayName.write(alarmDispL4);

}

void displayApneaAlarm(LiquidCrystal &displayName) { //Currently unused
	
	const char alarmDispL1[] = "ALARM CONDITION:";
	const char alarmDispL3[] = "APNEA";
	const char alarmDispL4[] = "NO BREATH INITIATED";

	displayName.clear();
	displayName.write(alarmDispL1);
	displayName.setCursor(0,2);
	displayName.write(alarmDispL3);
	displayName.setCursor(0,3);
	displayName.write(alarmDispL4);

}
void displayDeviceFailureAlarm(LiquidCrystal &displayName) {
	
	const char alarmDispL1[] = "ALARM CONDITION:";
	const char alarmDispL3[] = "UNRECOVERABLE ERROR";
	const char alarmDispL4[] = "POWER CYCLE REQUIRED";

	displayName.clear();
	displayName.write(alarmDispL1);
	displayName.setCursor(0,2);
	displayName.write(alarmDispL3);
	displayName.setCursor(0,3);
	displayName.write(alarmDispL4);

}


//Alarm setpoint change functions

void displayHighPressureChange(LiquidCrystal &displayName, float tempHighPressure, const int LCD_MAX_STRING) {

	int displayPressure = roundAndCast(tempHighPressure);

	const char alarmDispL1[] = "PRESS TO SET";
	const char alarmDispL3[] = "HIGH PIP LIMIT";
	char alarmDispL4[LCD_MAX_STRING];
	snprintf(alarmDispL4, LCD_MAX_STRING, "PRESSURE=%3d CM", displayPressure);

	displayName.clear();
	displayName.write(alarmDispL1);
	displayName.setCursor(0,2);
	displayName.write(alarmDispL3);
	displayName.setCursor(0,3);
	displayName.write(alarmDispL4);

}

void displayLowPressureChange(LiquidCrystal &displayName, float tempLowPressure, const int LCD_MAX_STRING) {

	int displayPressure = roundAndCast(tempLowPressure);

	const char alarmDispL1[] = "PRESS TO SET";
	const char alarmDispL3[] = "LOW PIP LIMIT";
	char alarmDispL4[LCD_MAX_STRING];
	snprintf(alarmDispL4, LCD_MAX_STRING, "PRESSURE=%3d CM", displayPressure);

	displayName.clear();
	displayName.write(alarmDispL1);
	displayName.setCursor(0,2);
	displayName.write(alarmDispL3);
	displayName.setCursor(0,3);
	displayName.write(alarmDispL4);

}

void displayHighPEEPChange(LiquidCrystal &displayName, float tempHighPEEP, const int LCD_MAX_STRING) {

	int displayPressure = roundAndCast(tempHighPEEP);

	const char alarmDispL1[] = "PRESS TO SET";
	const char alarmDispL3[] = "HIGH PEEP LIMIT";
	char alarmDispL4[LCD_MAX_STRING];
	snprintf(alarmDispL4, LCD_MAX_STRING, "PRESSURE=%3d CM", displayPressure);

	displayName.clear();
	displayName.write(alarmDispL1);
	displayName.setCursor(0,2);
	displayName.write(alarmDispL3);
	displayName.setCursor(0,3);
	displayName.write(alarmDispL4); 
}

void displayLowPEEPChange(LiquidCrystal &displayName, float tempLowPEEP, const int LCD_MAX_STRING) {

	int displayPressure = roundAndCast(tempLowPEEP);

	const char alarmDispL1[] = "PRESS TO SET";
	const char alarmDispL3[] = "LOW PEEP LIMIT";
	char alarmDispL4[LCD_MAX_STRING];
	snprintf(alarmDispL4, LCD_MAX_STRING, "PRESSURE=%3d CM", displayPressure);

	displayName.clear();
	displayName.write(alarmDispL1);
	displayName.setCursor(0,2);
	displayName.write(alarmDispL3);
	displayName.setCursor(0,3);
	displayName.write(alarmDispL4); 
}

void dipslayLowPlateauChange(LiquidCrystal &displayName, float tempLowPlateau, const int LCD_MAX_STRING) {

	int displayPressure = roundAndCast(tempLowPlateau);

	const char alarmDispL1[] = "PRESS TO SET";
	const char alarmDispL3[] = "LOW PLATEAU PRESSURE";
	char alarmDispL4[LCD_MAX_STRING];
	snprintf(alarmDispL4, LCD_MAX_STRING, "PRESSURE=%3d CM", displayPressure);

	displayName.clear();
	displayName.write(alarmDispL1);
	displayName.setCursor(0,2);
	displayName.write(alarmDispL3);
	displayName.setCursor(0,3);
	displayName.write(alarmDispL4);
}
//End of Alarm Screen Functions

//Parameter display functions

void displayVentilationParameters(LiquidCrystal &displayName, machineStates machineState, vcModeStates vcState , acModeStates acState, float breathsPerMinute, float thresholdPressure, float tidalVolume, float inspirationTime, float inspirationPause, float measuredPIP, float measuredPlateau, const int LCD_MAX_STRING) {

	int displayBPM = roundAndCast(breathsPerMinute);
	int displayThresholdPressure = roundAndCast(thresholdPressure);
	int displayTV = roundAndCast(tidalVolume);
	int displayITFirstDigit = (int) inspirationTime;
	int displayITSecondDigit = getFirstDigitPastDecimal(inspirationTime);
	int displayIPFirstDigit = getFirstDigitPastDecimal(inspirationPause);
	int displayIPSecondDigit = getSecondDigitPastDecimal(inspirationPause);
	int displayPIP = roundAndCast(measuredPIP);
	int displayPlateau = roundAndCast(measuredPlateau);
	int displayVCStateCode = vcCodeAssignment(vcState);
	int displayACStateCode = acCodeAssignment(acState);
	char displayMachineStateCode = machineStateCodeAssignment(machineState);

	// TODO: This makes me nervous...
	char displayVentilatorMode[5];
	if('A' == displayMachineStateCode){
		strcpy(displayVentilatorMode,"AC");
	}
	else if('V' == displayMachineStateCode){
		strcpy(displayVentilatorMode,"VC");
	}
	else{
		strcpy(displayVentilatorMode,"--");
	}

	//TODO
	char parameterDispL1[LCD_MAX_STRING];
	char parameterDispL2[LCD_MAX_STRING];
	char parameterDispL3[LCD_MAX_STRING];
	char parameterDispL4[LCD_MAX_STRING];

	snprintf(parameterDispL1, LCD_MAX_STRING, "MODE:%-3s|BPM=%2d  %1c%1d%1d", displayVentilatorMode, displayBPM, displayMachineStateCode, displayACStateCode, displayVCStateCode);
	snprintf(parameterDispL2, LCD_MAX_STRING, "TP=%2dCM |TV=%3d%%", displayThresholdPressure, displayTV);
	snprintf(parameterDispL3, LCD_MAX_STRING, "IT=%1d.%1ds |PAUSE 0.%1d%1ds", displayITFirstDigit, displayITSecondDigit, displayIPFirstDigit, displayIPSecondDigit);
	snprintf(parameterDispL4, LCD_MAX_STRING, "PIP=%2dCM|PLAT=%2dCM", displayPIP, displayPlateau);

	displayName.clear();
	displayName.write(parameterDispL1);
	displayName.setCursor(0,1);
	displayName.write(parameterDispL2);
	displayName.setCursor(0,2);
	displayName.write(parameterDispL3);
	displayName.setCursor(0,3);
	displayName.write(parameterDispL4);

} 

void displayStartupScreen(LiquidCrystal &displayName, const char softwareVersion[], const int LCD_MAX_STRING) {

	const char parameterDispL1[] = "EMERGENCY VENTILATOR";
	char parameterDispL3[LCD_MAX_STRING];

	snprintf(parameterDispL3, LCD_MAX_STRING, "%s",softwareVersion);

	displayName.clear();
	displayName.write(parameterDispL1);
	displayName.setCursor(0,2);
	displayName.write(parameterDispL3);

	delay(2000);

}

void displayHomingScreen(LiquidCrystal &displayName) {

	const char parameterDispL1[] = "CALIBRATION";
	const char parameterDispL2[] = "IN PROGRESS...";

	displayName.clear();
	displayName.write(parameterDispL1);
	displayName.setCursor(0,1);
	displayName.write(parameterDispL2);

}



//Parameter setpoint change functions

void displayTVChange(LiquidCrystal &displayName, float tempTV, const int LCD_MAX_STRING) {

	int displayTV = roundAndCast(tempTV);

	const char parameterDispL1[] = "PRESS TO SET";
	const char parameterDispL3[] = "TIDAL VOLUME SETTING";
	char parameterDispL4[LCD_MAX_STRING];
	snprintf(parameterDispL4, LCD_MAX_STRING, "=%d%%", displayTV);

	displayName.clear();
	displayName.write(parameterDispL1);
	displayName.setCursor(0,2);
	displayName.write(parameterDispL3);
	displayName.setCursor(0,3);
	displayName.write(parameterDispL4);

}

void displayBPMChange(LiquidCrystal &displayName, float tempBPM, const int LCD_MAX_STRING) {

	int displayBPM = roundAndCast(tempBPM);

	const char parameterDispL1[] = "PRESS TO SET";
	const char parameterDispL3[] = "BREATHS/MIN SETTING";
	char parameterDispL4[LCD_MAX_STRING];
	snprintf(parameterDispL4, LCD_MAX_STRING, "=%2d/MIN", displayBPM);

	displayName.clear();
	displayName.write(parameterDispL1);
	displayName.setCursor(0,2);
	displayName.write(parameterDispL3);
	displayName.setCursor(0,3);
	displayName.write(parameterDispL4);

}

void displayInspirationTimeChange(LiquidCrystal &displayName, float tempIT, const int LCD_MAX_STRING) {

	int displayITFirstDigit = (int) tempIT;
	int displayITSecondDigit = getFirstDigitPastDecimal(tempIT);

	const char parameterDispL1[] = "PRESS TO SET";
	const char parameterDispL3[] = "INSPIRATION TIME";
	char parameterDispL4[LCD_MAX_STRING];
	snprintf(parameterDispL4, LCD_MAX_STRING, "=%d.%d SEC", displayITFirstDigit, displayITSecondDigit);

	displayName.clear();
	displayName.write(parameterDispL1);
	displayName.setCursor(0,2);
	displayName.write(parameterDispL3);
	displayName.setCursor(0,3);
	displayName.write(parameterDispL4);

}

void displayPauseTimeChange(LiquidCrystal &displayName, float tempPauseTime, const int LCD_MAX_STRING) {

	int displayPTLeadDigit = getFirstDigitPastDecimal(tempPauseTime);
	int displayPTLastDigit = (int) (10*(10*tempPauseTime - displayPTLeadDigit));

	const char parameterDispL1[] = "PRESS TO SET";
	const char parameterDispL3[] = "PLATEAU PAUSE";
	char parameterDispL4[LCD_MAX_STRING];
	snprintf(parameterDispL4, LCD_MAX_STRING, "=0.%d%d SEC", displayPTLeadDigit, displayPTLastDigit);

	displayName.clear();
	displayName.write(parameterDispL1);
	displayName.setCursor(0,2);
	displayName.write(parameterDispL3);
	displayName.setCursor(0,3);
	displayName.write(parameterDispL4);

}

void displayThresholdPressureChange(LiquidCrystal &displayName, float tempThresholdPressure, const int LCD_MAX_STRING) {

	int displayThresholdPressure = roundAndCast(tempThresholdPressure);

	const char parameterDispL1[] = "PRESS TO SET";
	const char parameterDispL3[] = "THRESHOLD PRESSURE";
	char parameterDispL4[LCD_MAX_STRING];
	snprintf(parameterDispL4, LCD_MAX_STRING, "=%d CM", displayThresholdPressure);

	displayName.clear();
	displayName.write(parameterDispL1);
	displayName.setCursor(0,2);
	displayName.write(parameterDispL3);
	displayName.setCursor(0,3);
	displayName.write(parameterDispL4);

}

//Helper Functions

int roundAndCast(float x) {
	int new_var;
  	new_var = (int) round(x);
  	return new_var;
}


int getFirstDigitPastDecimal(float realNumber) {
	int naturalNumber = (int) realNumber;
	int firstDigitPastDecimal = (int) (10*(realNumber - naturalNumber));

	return firstDigitPastDecimal;
}


int getSecondDigitPastDecimal(float realNumber) {
	int naturalNumber = 10*((int) (10*realNumber));
	int secondDigitPastDecimal = (int) (100*realNumber - naturalNumber);

	return secondDigitPastDecimal;
}
