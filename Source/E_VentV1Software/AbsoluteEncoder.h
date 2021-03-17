#ifndef AbsoluteEncoder_h
#define AbsoluteEncoder_h

#include "PinAssignments.h"

//Arduino library required for analogRead
#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

const float ADC_MAX_VALUE = 1023.0;
const float ADC_MAX_VOLTAGE = 5.0;

struct AbsoluteEncoderStruct{

	float zeroPointVoltage;
	float voltageToTicksSlope;
	float voltageToTicksIntercept;

};

AbsoluteEncoderStruct setupAbsoluteEncoder(void);

void setAbsZeroPointVoltage(AbsoluteEncoderStruct &absEncoder);

void setAbsVoltageToTicks(AbsoluteEncoderStruct &absEncoder, const long int motorPosition);

float readAbsoluteEncoder(void);

float convertADCToVolts(long adcReading);

float readAbsoluteEncoderTicks(AbsoluteEncoderStruct &absEncoder);


#endif