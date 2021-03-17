#include "AbsoluteEncoder.h"


AbsoluteEncoderStruct setupAbsoluteEncoder(void){
	AbsoluteEncoderStruct absEncoder;

	absEncoder.zeroPointVoltage = 0.0;
	absEncoder.voltageToTicksSlope = 0.0;
	absEncoder.voltageToTicksIntercept = 0.0;

	return absEncoder;
}


void setAbsZeroPointVoltage(AbsoluteEncoderStruct &absEncoder){
	//This function should be called after the limit switch is hit
	absEncoder.zeroPointVoltage = readAbsoluteEncoder();
	return;
}

void setAbsVoltageToTicks(AbsoluteEncoderStruct &absEncoder, const long int motorPosition){
	//This function should be called after zeroing the motor encoder and then moving in a set amount of ticks
	float absEncoderVoltage = readAbsoluteEncoder(); //Returns current voltage reading
	absEncoder.voltageToTicksSlope = motorPosition/(absEncoderVoltage - absEncoder.zeroPointVoltage);
	absEncoder.voltageToTicksIntercept = -1*absEncoder.zeroPointVoltage*absEncoder.voltageToTicksSlope;
	return;
}

float readAbsoluteEncoder(void){
	long adcReading = analogRead(ABS_ENCODER_PIN);
	return convertADCToVolts(adcReading);
}

float convertADCToVolts(long adcReading){
	float voltageReading = adcReading/ADC_MAX_VALUE*ADC_MAX_VOLTAGE;
	return voltageReading;
}

float readAbsoluteEncoderTicks(AbsoluteEncoderStruct &absEncoder){
	//Convert from an encoder voltage to ticks
	float currentTicks = readAbsoluteEncoder()*absEncoder.voltageToTicksSlope + absEncoder.voltageToTicksIntercept;
	return currentTicks;
}
