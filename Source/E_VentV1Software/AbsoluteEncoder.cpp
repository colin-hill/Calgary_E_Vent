#include "AbsoluteEncoder.h"


void setZeroPointVoltage(float &zeroPointVoltage){
	//This function should be called after the limit switch is hit
	zeroPointVoltage = readAbsoluteEncoder();
	return;
}

void setVoltageToTicks(float &voltageToTicksSlope, float &voltageToTicksIntercept, const long int motorPosition, const float zeroPointVoltage){
	//This function should be called after zeroing the motor encoder and then moving in a set amount of ticks
	float absEncoderVoltage = readAbsoluteEncoder(); //Returns current voltage reading
	voltageToTicksSlope = motorPosition/(absEncoderVoltage - zeroPointVoltage);
	voltageToTicksIntercept = -1*zeroPointVoltage*voltageToTicksSlope;
	return;
}


float readAbsoluteEncoder(){
	long adcReading = analogRead(ABS_ENCODER_PIN);
	return convertADCToVolts(adcReading);
}

float convertADCToVolts(long adcReading){
	float voltageReading = adcReading/ADC_MAX_VALUE*ADC_MAX_VOLTAGE;
	return voltageReading;
}

float readAbsoluteEncoderTicks(const float voltageToTicks, const float voltageToTicksIntercept){
	//Convert from an encoder voltage to ticks
	float currentTicks = readAbsoluteEncoder()*voltageToTicksSlope + voltageToTicksIntercept;
	return currentTicks;
}
