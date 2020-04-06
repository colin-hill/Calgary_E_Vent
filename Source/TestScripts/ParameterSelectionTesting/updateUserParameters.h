#ifndef UPDATE_USER_PARAMETERS_H
#define UPDATE_USER_PARAMETERS_H

#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include "UserParameter.h"
#include "Encoder.h"

const uint8_t parameterSelectInterruptPin = 2;
const uint8_t parameterEncoderPushButtonPin = 20;
const uint8_t parameterEncoderPin1 = 18;
const uint8_t parameterEncoderPin2 = 19;

//volatile boolean PARAMETER_SELECT = false;
//volatile boolean PARAMETER_SET = false;

//Encoder PARAMETER_SELECT_ENCODER(parameterEncoderPin1, parameterEncoderPin2);

enum SelectedParameter{
	e_None,
	e_ThresholdPressure,
	e_BPM,
	e_IERatio,
	e_TidalVolume
};

//SelectedParameter CURRENTLY_SELECTED_PARAMETER = e_None;

void setUpParameterSelectButtons(UserParameter &thresholdPressure, UserParameter &bpm, UserParameter &ieRatio, UserParameter &tidalVolume,
                                const uint8_t parameterSelectInterrptPin, const uint8_t parameterEncoderPushButtonPin);

void updateSelectedParameter(SelectedParameter &currentlySelectedParameter, 
							volatile boolean &parameterSelect, 
							Encoder &parameterSelectEncoder, UserParameter &thresholdPressure, UserParameter &bpm,
							UserParameter &ieRatio, UserParameter &tidalVolume);
							
void updateParameterValue(SelectedParameter &currentlySelectedParameter, 
						Encoder &parameterSelectEncoder, UserParameter &thresholdPressure, UserParameter &bpm,
						UserParameter &ieRatio, UserParameter &tidalVolume);

void setParameters(SelectedParameter &currentlySelectedParameter, volatile boolean &parameterSelect,
				volatile boolean &parameterSet, UserParameter &thresholdPressure, UserParameter &bpm, UserParameter &ieRatio,
				UserParameter &tidalVolume);

void updateUserParameters(SelectedParameter &currentlySelectedParameter, volatile boolean &parameterSelect, volatile boolean &parameterSet,
            Encoder &parameterSelectEncoder, UserParameter &thresholdPressure, UserParameter &bpm,
            UserParameter &ieRatio, UserParameter &tidalVolume);

void parameterSelectISR();

void parameterSetISR();

#endif
