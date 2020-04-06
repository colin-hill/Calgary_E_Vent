#include "LCD.h"


//Alarm Display Functions

void displayNoAlarm(LiquidCrystal &displayName, float highPressure, float lowPressure, float highPEEP, float lowPEEP, float lowPlateau, int lcdColumns) {
	
	int displayHighPressure = roundAndCast(highPressure);
	int displayLowPressure = roundAndCast(lowPressure);
	int displayHighPEEP = roundAndCast(highPEEP);
	int displayLowPEEP = roundAndCast(lowPEEP);
	int displayLowPlateau = roundAndCast(lowPlateau);

	char alarmDispL1[] = "NO ALARM  SETPOINTS:";
	char alarmDispL2[25];
	char alarmDispL3[25];
	char alarmDispL4[25];
	snprintf(alarmDispL2, lcdColumns, "PIP=%11d-%2dcm", displayLowPressure, displayHighPressure);
	snprintf(alarmDispL3, lcdColumns, "PEEP=%10d-%2dcm", displayLowPEEP, displayHighPEEP);
	snprintf(alarmDispL4, lcdColumns, "PLATEAU MIN=%6dcm", displayLowPlateau);

	displayName.clear();
	displayName.write(alarmDispL1);
	displayName.setCursor(0,1);
	displayName.write(alarmDispL2);
	displayName.setCursor(0,2);
	displayName.write(alarmDispL3);
	displayName.setCursor(0,3);
	displayName.write(alarmDispL4);

}


void displayHighPressureAlarm(LiquidCrystal &displayName, float pressureMeasurement, int lcdColumns) {
	
	int displayPressure = roundAndCast(pressureMeasurement);

	char alarmDispL1[] = "ALARM CONDITION:";
	char alarmDispL3[] = "HIGH INSPIRATION";
	char alarmDispL4[25];
	snprintf(alarmDispL4, lcdColumns, "PRESSURE=%3d CM H2O", displayPressure);

	displayName.clear();
	displayName.write(alarmDispL1);
	displayName.setCursor(0,2);
	displayName.write(alarmDispL3);
	displayName.setCursor(0,3);
	displayName.write(alarmDispL4);

}

void displayLowPressureAlarm(LiquidCrystal &displayName, float pressureMeasurement, int lcdColumns) {
	
	int displayPressure = roundAndCast(pressureMeasurement);

	char alarmDispL1[] = "ALARM CONDITION:";
	char alarmDispL3[] = "LOW INSPIRATION";
	char alarmDispL4[25];
	snprintf(alarmDispL4, lcdColumns, "PRESSURE=%3d CM H2O", displayPressure);

	displayName.clear();
	displayName.write(alarmDispL1);
	displayName.setCursor(0,2);
	displayName.write(alarmDispL3);
	displayName.setCursor(0,3);
	displayName.write(alarmDispL4);

}

void displayHighPEEPAlarm(LiquidCrystal &displayName, float pressureMeasurement, int lcdColumns) {
	
	int displayPressure = roundAndCast(pressureMeasurement);

	char alarmDispL1[] = "ALARM CONDITION:";
	char alarmDispL3[] = "HIGH PEEP";
	char alarmDispL4[25];
	snprintf(alarmDispL4, lcdColumns, "PRESSURE=%3d CM H2O", displayPressure);

	displayName.clear();
	displayName.write(alarmDispL1);
	displayName.setCursor(0,2);
	displayName.write(alarmDispL3);
	displayName.setCursor(0,3);
	displayName.write(alarmDispL4);

}

void displayLowPEEPAlarm(LiquidCrystal &displayName, float pressureMeasurement, int lcdColumns) {
	
	int displayPressure = roundAndCast(pressureMeasurement);

	char alarmDispL1[] = "ALARM CONDITION:";
	char alarmDispL3[] = "LOW PEEP";
	char alarmDispL4[25];
	snprintf(alarmDispL4, lcdColumns, "PRESSURE=%3d CM H2O", displayPressure);

	displayName.clear();
	displayName.write(alarmDispL1);
	displayName.setCursor(0,2);
	displayName.write(alarmDispL3);
	displayName.setCursor(0,3);
	displayName.write(alarmDispL4);

}


void displayDisconnectAlarm(LiquidCrystal &displayName) {
	
	char alarmDispL1[] = "ALARM CONDITION:";
	char alarmDispL2[] = "POSSIBLE DISCONNECT";
	char alarmDispL3[] = "CHECK O2 AND AIR";
	char alarmDispL4[] = "CONNECTIONS";

	displayName.clear();
	displayName.write(alarmDispL1);
	displayName.setCursor(0,1);
	displayName.write(alarmDispL2);
	displayName.setCursor(0,2);
	displayName.write(alarmDispL3);
	displayName.setCursor(0,3);
	displayName.write(alarmDispL4);

}


void displayTemperatureAlarm(LiquidCrystal &displayName, float temperatureMeasurement, int lcdColumns) {
	
	int displayTemperature = roundAndCast(temperatureMeasurement);

	char alarmDispL1[] = "ALARM CONDITION:";
	char alarmDispL3[] = "HIGH CONTROLLER TEMP";
	char alarmDispL4[25];
	snprintf(alarmDispL4, lcdColumns, "TEMPERATURE=%3d C", displayTemperature);

	displayName.clear();
	displayName.write(alarmDispL1);
	displayName.setCursor(0,2);
	displayName.write(alarmDispL3);
	displayName.setCursor(0,3);
	displayName.write(alarmDispL4);

}

void displayApneaAlarm(LiquidCrystal &displayName) { //Currently unused
	
	char alarmDispL1[] = "ALARM CONDITION:";
	char alarmDispL3[] = "APNEA";
	char alarmDispL4[] = "NO BREATH INITIATED";

	displayName.clear();
	displayName.write(alarmDispL1);
	displayName.setCursor(0,2);
	displayName.write(alarmDispL3);
	displayName.setCursor(0,3);
	displayName.write(alarmDispL4);

}
void displayDeviceFailureAlarm(LiquidCrystal &displayName) {
	
	char alarmDispL1[] = "ALARM CONDITION:";
	char alarmDispL3[] = "UNRECOVERABLE ERROR";
	char alarmDispL4[] = "POWER CYCLE REQUIRED";

	displayName.clear();
	displayName.write(alarmDispL1);
	displayName.setCursor(0,2);
	displayName.write(alarmDispL3);
	displayName.setCursor(0,3);
	displayName.write(alarmDispL4);

}


//Alarm setpoint change functions

void displayHighPressureChange(LiquidCrystal &displayName, float tempHighPressure, int lcdColumns) {

	int displayPressure = roundAndCast(tempHighPressure);

	char alarmDispL1[] = "PRESS TO SET";
	char alarmDispL3[] = "HIGH PIP LIMIT";
	char alarmDispL4[25];
	snprintf(alarmDispL4, lcdColumns, "PRESSURE=%3d CM", displayPressure);

	displayName.clear();
	displayName.write(alarmDispL1);
	displayName.setCursor(0,2);
	displayName.write(alarmDispL3);
	displayName.setCursor(0,3);
	displayName.write(alarmDispL4);

}

void displayLowPressureChange(LiquidCrystal &displayName, float tempLowPressure, int lcdColumns) {

	int displayPressure = roundAndCast(tempLowPressure);

	char alarmDispL1[] = "PRESS TO SET";
	char alarmDispL3[] = "LOW PIP LIMIT";
	char alarmDispL4[25];
	snprintf(alarmDispL4, lcdColumns, "PRESSURE=%3d CM", displayPressure);

	displayName.clear();
	displayName.write(alarmDispL1);
	displayName.setCursor(0,2);
	displayName.write(alarmDispL3);
	displayName.setCursor(0,3);
	displayName.write(alarmDispL4);

}

void displayHighPEEPChange(LiquidCrystal &displayName, float tempHighPEEP, int lcdColumns) {

	int displayPressure = roundAndCast(tempHighPEEP);

	char alarmDispL1[] = "PRESS TO SET";
	char alarmDispL3[] = "HIGH PEEP LIMIT";
	char alarmDispL4[25];
	snprintf(alarmDispL4, lcdColumns, "PRESSURE=%3d CM", displayPressure);

	displayName.clear();
	displayName.write(alarmDispL1);
	displayName.setCursor(0,2);
	displayName.write(alarmDispL3);
	displayName.setCursor(0,3);
	displayName.write(alarmDispL4); 
}

void displayLowPEEPChange(LiquidCrystal &displayName, float tempLowPEEP, int lcdColumns) {

	int displayPressure = roundAndCast(tempLowPEEP);

	char alarmDispL1[] = "PRESS TO SET";
	char alarmDispL3[] = "LOW PEEP LIMIT";
	char alarmDispL4[25];
	snprintf(alarmDispL4, lcdColumns, "PRESSURE=%3d CM", displayPressure);

	displayName.clear();
	displayName.write(alarmDispL1);
	displayName.setCursor(0,2);
	displayName.write(alarmDispL3);
	displayName.setCursor(0,3);
	displayName.write(alarmDispL4); 
}

void dipslayLowPlateauChange(LiquidCrystal &displayName, float tempLowPlateau, int lcdColumns) {

	int displayPressure = roundAndCast(tempLowPlateau);

	char alarmDispL1[] = "PRESS TO SET";
	char alarmDispL3[] = "LOW PLATEAU PRESSURE";
	char alarmDispL4[25];
	snprintf(alarmDispL4, lcdColumns, "PRESSURE=%3d CM", displayPressure);

	displayName.clear();
	displayName.write(alarmDispL1);
	displayName.setCursor(0,2);
	displayName.write(alarmDispL3);
	displayName.setCursor(0,3);
	displayName.write(alarmDispL4);
}


//Parameter display functions

void displayVentilationParameters(LiquidCrystal &displayName, machineStates machineState, vcModeStates vcState , acModeStates acState, float breathsPerMinute, float thresholdPressure, float tidalVolume, float inspirationTime, float inspirationPause, float measuredPIP, float measuredPlateau, int lcdColumns) {

	int displayBPM = roundAndCast(breathsPerMinute);
	int displayThresholdPressure = roundAndCast(thresholdPressure);
	int displayTV = roundAndCast(tidalVolume);
	//int displayIT = roundAndCast(inspirationTime);
	//int displayIP = roundAndCast(inspirationPause);
	int displayPIP = roundAndCast(measuredPIP);
	int displayPlateau = roundAndCast(measuredPlateau);

	char machineStateCode = machineStateCodeAssignment(machineState);
	int  vcStateCode = vcCodeAssignment(vcState);
	int acStateCode = acCodeAssignment(acState);

	//TODO




} 



//Parameter setpoint change functions

void displayTVChange(LiquidCrystal &displayName, float tempTV, int lcdColumns) {

	int displayTV = roundAndCast(tempTV);

	char parameterDispL1[] = "PRESS TO SET";
	char parameterDispL3[] = "TIDAL VOLUME SETTING";
	char parameterDispL4[25];
	snprintf(parameterDispL4, lcdColumns, "=%d%%", displayTV);

	displayName.clear();
	displayName.write(parameterDispL1);
	displayName.setCursor(0,2);
	displayName.write(parameterDispL3);
	displayName.setCursor(0,3);
	displayName.write(parameterDispL4);

}

void displayBPMChange(LiquidCrystal &displayName, float tempBPM, int lcdColumns) {

	int displayBPM = roundAndCast(tempBPM);

	char parameterDispL1[] = "PRESS TO SET";
	char parameterDispL3[] = "BREATHS/MIN SETTING";
	char parameterDispL4[25];
	snprintf(parameterDispL4, lcdColumns, "=%2d/MIN", displayBPM);

	displayName.clear();
	displayName.write(parameterDispL1);
	displayName.setCursor(0,2);
	displayName.write(parameterDispL3);
	displayName.setCursor(0,3);
	displayName.write(parameterDispL4);

}

void displayInspirationTimeChange(LiquidCrystal &displayName, float tempIT, int lcdColumns) {

	int displayITLeadDigit = (int) tempIT;
	int displayITLastDigit = (int) (10*(tempIT - displayITLeadDigit));

	char parameterDispL1[] = "PRESS TO SET";
	char parameterDispL3[] = "INSPIRATION TIME";
	char parameterDispL4[25];
	snprintf(parameterDispL4, lcdColumns, "=%d.%d SEC", displayITLeadDigit, displayITLastDigit);

	displayName.clear();
	displayName.write(parameterDispL1);
	displayName.setCursor(0,2);
	displayName.write(parameterDispL3);
	displayName.setCursor(0,3);
	displayName.write(parameterDispL4);

}

void displayPauseTimeChange(LiquidCrystal &displayName, float tempPauseTime, int lcdColumns) {

	int displayPTLeadDigit = (int) (10*tempPauseTime);
	int displayPTLastDigit = (int) (10*(10*tempPauseTime - displayPTLeadDigit));

	char parameterDispL1[] = "PRESS TO SET";
	char parameterDispL3[] = "PLATEAU PAUSE";
	char parameterDispL4[25];
	snprintf(parameterDispL4, lcdColumns, "=0.%d%d SEC", displayPTLeadDigit, displayPTLastDigit);

	displayName.clear();
	displayName.write(parameterDispL1);
	displayName.setCursor(0,2);
	displayName.write(parameterDispL3);
	displayName.setCursor(0,3);
	displayName.write(parameterDispL4);

}

void displayThresholdPressureChange(LiquidCrystal &displayName, float tempThresholdPressure, int lcdColumns) {

	int displayThresholdPressure = roundAndCast(tempThresholdPressure);

	char parameterDispL1[] = "PRESS TO SET";
	char parameterDispL3[] = "THRESHOLD PRESSURE";
	char parameterDispL4[25];
	snprintf(parameterDispL4, lcdColumns, "=%d CM", displayThresholdPressure);

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

char machineStateCodeAssignment(machineStates machineState) {

	char machineStateCode;

	if(Startup == machineState){
		machineStateCode = 'A';
	}
	else if(MotorZeroing == machineState){
		machineStateCode = 'B';
	}
	else if(BreathLoopStart == machineState){
		machineStateCode = 'C';
	}
	else if(ACMode == machineState){
		machineStateCode = 'D';
	}
	else if(VCMode == machineState){
		machineStateCode = 'E';
	}
	else{//Machine Failure
		machineStateCode = 'F';
	}

	return machineStateCode;
}


int vcCodeAssignment(vcModeStates vcState) {

	int vcStateCode;

	if(VCStart == vcState){
		vcStateCode = 1;
	}
	else if(VCInhale == vcState){
		vcStateCode = 2;
	}
	else if(VCInhaleCommand == vcState){
		vcStateCode = 3;
	}
	else if(VCPeak == vcState){
		vcStateCode = 4;
	}
	else if(VCExhaleCommand){
		vcStateCode = 5;
	}
	else if(VCExhale == vcState){
		vcStateCode = 6;
	}
	else if(VCReset == vcState){
		vcStateCode = 7;
	}
	else{//VC Inhale abort
		vcStateCode = 8;
	}
} 

int acCodeAssignment(acModeStates acState) {

	int acStateCode;

	if(ACStart == acState){
		acStateCode = 1;
	}
	else if(ACInhaleWait == acState){
		acStateCode = 2;
	}
	else if(ACInhaleCommand == acState){
		acStateCode = 3;
	}
	else if(ACInhale == acState){
		acStateCode = 4;
	}
	else if(ACPeak == acState){
		acStateCode = 5;
	}
	else if(ACExhaleCommand == acState){
		acStateCode = 6;
	}
	else if(ACExhale == acState){
		acStateCode = 7;
	}
	else if(ACReset == acState){
		acStateCode = 8;
	}
	else{//AC Inhale abort
		acStateCode = 9;
	}


}
