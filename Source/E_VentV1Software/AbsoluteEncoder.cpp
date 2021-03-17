#include "AbsoluteEncoder.h"


void setZeroPointVoltage(float &zeroPointVoltage){
	//This function should be called after the limit switch is hit
	zeroPointVoltage = readAbsoluteEncoder();
	return;
}

void setVoltageToTicks(float &voltageToTicks,const long int motorPosition, const float zeroPointVoltage){
	//This function should be called after zeroing the motor encoder and then moving in a set amount of ticks
	float absEncoderVoltage = readAbsoluteEncoder(); //Returns current voltage reading
	voltageToTicks = motorPosition/(absEncoderVoltage - zeroPointVoltage);
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

float readAbsoluteEncoderTicks(const float voltageToTicks){
	//Convert from an encoder voltage to ticks
	float currentTicks = readAbsoluteEncoder()*voltageToTicks;
	return currentTicks;
}
