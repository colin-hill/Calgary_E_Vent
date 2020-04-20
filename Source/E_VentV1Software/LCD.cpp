#include "LCD.h"

//Alarm Display Functions

void displayNoAlarm(LiquidCrystal &displayName, float highPressure, float lowPressure, float highPEEP, float lowPEEP, float lowPlateau, const int LCD_MAX_STRING) {
    
    char alarmStr[LCD_MAX_STRING];
	
	int displayHighPressure = roundAndCast(highPressure);
	int displayLowPressure = roundAndCast(lowPressure);
	int displayHighPEEP = roundAndCast(highPEEP);
	int displayLowPEEP = roundAndCast(lowPEEP);
	//int displayLowPlateau = roundAndCast(lowPlateau);

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
	//displayName.setCursor(0,3);
	//snprintf(alarmStr, LCD_MAX_STRING, "PLATEAU MIN=%6dCM", displayLowPlateau);
	//displayName.write(alarmStr);
}


void displayMultipleAlarms(LiquidCrystal &displayName, float  maxPIP, float minPIP, float maxPEEP, float minPEEP, float respiratoryRate, VentilatorState &state) {

	const char alarmDispL1[] = "ALARM OVERVIEW:";
	char alarmStr[LCD_MAX_STRING];

	int displayHighPIP = roundAndCast(maxPIP);
	int displayLowPIP = roundAndCast(minPIP);
	int displayHighPEEP = roundAndCast(maxPEEP);
	int displayLowPEEP = roundAndCast(minPEEP);
	int displayRespiratoryRate = roundAndCast(respiratoryRate);

	displayName.createChar(0, DISP_LED_OFF);
	displayName.createChar(1, DISP_LED_ON);


	displayName.clear();
	displayName.write(alarmDispL1);

	//Line 2
	displayName.setCursor(0,1);
	snprintf(alarmStr, LCD_MAX_STRING, "HPI(%2d) ", displayHighPIP);
	displayName.write(alarmStr);
	if (state.alarm_outputs & HIGH_PRESSURE_ALARM) {
		displayName.write((byte)1);
	}
	else {
		displayName.write((byte)0);
	}
	snprintf(alarmStr, LCD_MAX_STRING, "| LPI(%2d) ", displayLowPIP);
	displayName.write(alarmStr);
	if (state.alarm_outputs & LOW_PRESSURE_ALARM) {
		displayName.write((byte)1);
	}
	else {
		displayName.write((byte)0);
	}	
	
	//Line 3
	displayName.setCursor(0,2);
	snprintf(alarmStr, LCD_MAX_STRING, "HPE(%2d) ", displayHighPEEP);
	displayName.write(alarmStr);
	if (state.alarm_outputs & HIGH_PEEP_ALARM) {
		displayName.write((byte)1);
	}
	else {
		displayName.write((byte)0);
	}
	snprintf(alarmStr, LCD_MAX_STRING, "| LPE(%2d) ", displayLowPEEP);
	displayName.write(alarmStr);
	if (state.alarm_outputs & LOW_PEEP_ALARM) {
		displayName.write((byte)1);
	}
	else {
		displayName.write((byte)0);
	}

	//Line 4
	displayName.setCursor(0,3);
	snprintf(alarmStr, LCD_MAX_STRING, "HRR(%2d) ", displayRespiratoryRate);
	displayName.write(alarmStr);
	if (state.alarm_outputs & HIGH_RR_ALARM) {
		displayName.write((byte)1);
	}
	else {
		displayName.write((byte)0);
	}

	/*snprintf(alarmStr, LCD_MAX_STRING, "| MSDTRIG ", displayHighPIP);
	displayName.write(alarmStr);
	if (state.alarm_outputs & MISSED_TRIGGER_ALARM) {
		displayName.write((byte)1);
	}
	else {
		displayName.write((byte)0);
	}
*/



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


void displayMechanicalFailureAlarm(LiquidCrystal &displayName) {
	
	const char alarmDispL1[] = "ALARM CONDITION:";
	const char alarmDispL2[] = "MOTOR LOCATION FAULT";
	const char alarmDispL3[] = "CALIBRATION";
	const char alarmDispL4[] = "IN PROGRESS...";

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

void displayLowPlateauChange(LiquidCrystal &displayName, float tempLowPlateau, const int LCD_MAX_STRING) {

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

void displayVentilationParameters(LiquidCrystal &displayName,
								  machineStates machineState,
								  vcModeStates vcState, 
								  acModeStates acState, 
								  float breathsPerMinute, float thresholdPressure, 
								  float tidalVolume, float inspirationTime, 
								  float livePressure, float measuredPIP, 
								  float measuredPlateau, const int LCD_MAX_STRING) {

	int displayBPM = roundAndCast(breathsPerMinute);
	int displayThresholdPressure = roundAndCast(thresholdPressure);
	int displayTV = roundAndCast(tidalVolume);
	int displayITFirstDigit = (int) inspirationTime;
	int displayITSecondDigit = getFirstDigitPastDecimal(inspirationTime);
	int displayMeasuredPressure = roundAndCast(livePressure);
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
	snprintf(parameterDispL3, LCD_MAX_STRING, "IT=%1d.%1ds |PRESS=%3dCM", displayITFirstDigit, displayITSecondDigit, displayMeasuredPressure);
	snprintf(parameterDispL4, LCD_MAX_STRING, "PIP=%2dCM|PEEP=%2dCM", displayPIP, displayPlateau);

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


void displayAEVStartupScreen(LiquidCrystal &displayName) {

	displayName.createChar(0,L1_1);
	displayName.createChar(1,L1_2);
	displayName.createChar(2,L2_1);
	displayName.createChar(3,L2_2);
	displayName.createChar(4,LSS);
	displayName.createChar(5,L3_1);
	displayName.createChar(6,L3_2);
	displayName.createChar(7,L4_1);

	
	const char parameterDispL2[] = "ALBERTA";
	const char parameterDispL3[] = "E-VENT";

	//Line 1
	displayName.clear();
	displayName.setCursor(5,0);
	displayName.write((byte)0);
	displayName.write((byte)1);

	//Line 2
	displayName.setCursor(4,1);
	displayName.write((byte)2);
	displayName.write((byte)3);
	displayName.write((byte)4);
	displayName.write(parameterDispL2);
	
	//Line 3
	displayName.setCursor(4,2);
	displayName.write((byte)5);
	displayName.write((byte)6);
	displayName.write((byte)4);
	displayName.write(parameterDispL3);


}

void displayStartupHoldScreen(LiquidCrystal &displayName) {

	const char parameterDispL1[] = "ENSURE PERSONNEL ARE";
	const char parameterDispL2[] = "CLEAR OF VENTILATOR";
	const char parameterDispL3[] = "PRESS ALARM DISMISS";
	const char parameterDispL4[] = "TO CONFIRM...";

	displayName.clear();
	displayName.write(parameterDispL1);
	displayName.setCursor(0,1);
	displayName.write(parameterDispL2);
	displayName.setCursor(0,2);
	displayName.write(parameterDispL3);
	displayName.setCursor(0,3);
	displayName.write(parameterDispL4);

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
