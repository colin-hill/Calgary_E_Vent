#ifndef AbsoluteEncoder_h
#define AbsoluteEncoder_h

//#define SERIAL_DEBUG
//#define PYTHON_DEBUG

#include "PinAssignments.h"
#include "Motor.h"
#include "MachineStates.h"
#include "alarms.h"

//Arduino library required for analogRead
#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

const float ADC_MAX_VALUE = 1023.0;
const float ADC_MAX_VOLTAGE = 5.0;
const float ABS_TO_QUAD_TOLERANCE = 25;
const float MIN_ABS_VOLT_CHANGE_AT_START = 0.1; 
const float MAX_ABS_VOLT_DEVIATION_AT_ZERO = 0.1;


void setAbsVoltageToTicks(AbsoluteEncoderStruct &absEncoder, const long int motorPosition);

float readAbsoluteEncoder(void);

float convertADCToVolts(long adcReading);

float readAbsoluteEncoderTicks(AbsoluteEncoderStruct &absEncoder);

uint16_t checkForStartupError(AbsoluteEncoderStruct &absEncoder);

uint16_t checkForRunningError(VentilatorState &state, float currentEncoderReading);

void handle_abs_motor_recalibration(RoboClaw &controller_name, VentilatorState &state);

void handle_absolute_encoder_zeroing(RoboClaw &controller_name, VentilatorState &state);

void handle_absolute_encoder_ACMode(RoboClaw &controller_name, VentilatorState &state);

void handle_absolute_encoder_VCMode(RoboClaw &controller_name, VentilatorState &state);

void handle_absolute_encoder(RoboClaw &controller_name, VentilatorState &state);


#endif
