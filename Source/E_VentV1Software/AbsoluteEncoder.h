#ifndef AbsoluteEncoder_h
#define AbsoluteEncoder_h

#include "PinAssignments.h"

const float ADC_MAX_VALUE = 1023.0;
const float ADC_MAX_VOLTAGE = 5.0;

float zeroPointVoltage;

float voltageToTicksSlope;

float voltageToTicksIntercept;

void setZeroPointVoltage(float &zeroPointVoltage);

void setVoltageToTicks(float &voltageToTicksSlope, float &voltageToTicksIntercept, const long int motorPosition, const float zeroPointVoltage);

float readAbsoluteEncoder();

float convertADCToVolts(long adcReading);

float readAbsoluteEncoderAngle(const float voltageToTicksSlope, const float voltageToTicksIntercept);


