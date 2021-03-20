#ifndef AbsoluteEncoder_h
#define AbsoluteEncoder_h

#define SERIAL_DEBUG

#include "PinAssignments.h"
#include "Motor.h"
#include "MachineStates.h"

//Arduino library required for analogRead
#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

const float ADC_MAX_VALUE = 1023.0;
const float ADC_MAX_VOLTAGE = 5.0;
const float ABS_TO_QUAD_TOLERANCE = 200;


void setAbsVoltageToTicks(AbsoluteEncoderStruct &absEncoder, const long int motorPosition);

float readAbsoluteEncoder(void);

float convertADCToVolts(long adcReading);

float readAbsoluteEncoderTicks(AbsoluteEncoderStruct &absEncoder);

void handle_abs_motor_recalibration(RoboClaw &controller_name, VentilatorState &state);

void handle_absolute_encoder_zeroing(RoboClaw &controller_name, VentilatorState &state);

void handle_absolute_encoder_ACMode(RoboClaw &controller_name, VentilatorState &state);

void handle_absolute_encoder_VCMode(RoboClaw &controller_name, VentilatorState &state);

void handle_absolute_encoder(RoboClaw &controller_name, VentilatorState &state);


#endif