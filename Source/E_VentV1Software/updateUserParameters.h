#ifndef UPDATE_USER_PARAMETERS_H
#define UPDATE_USER_PARAMETERS_H

#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include "UserParameter.h"
#include "src/Encoder/Encoder.h"
#include "LCD.h"
#include "MachineStates.h"

#define SWITCH_PRESS LOW

// TODO: All of this needs documentation.

void setUpParameterSelectButtons(UserParameter *userParameters,
    const uint8_t NUM_USER_PARAMETERS, const uint8_t parameterEncoderPushButtonPin);

void updateSelectedParameter(SelectedParameter &currentlySelectedParameter,
    Encoder &parameterSelectEncoder, UserParameter *userParameters, const uint8_t NUM_USER_PARAMETERS);

void updateParameterTempValue(SelectedParameter &currentlySelectedParameter,
    Encoder &parameterSelectEncoder, UserParameter *userParameter);

void setParameters(SelectedParameter &currentlySelectedParameter,
    volatile boolean &parameterSet, UserParameter *userParamter);

VentilatorState updateStateUserParameters(VentilatorState &state, SelectedParameter &currentlySelectedParameter,volatile boolean &parameterSet,
    Encoder &parameterSelectEncoder, UserParameter *userParameters, const uint8_t NUM_USER_PARAMETERS);

void displayUserParameters(SelectedParameter &currentlySelectedParameter, LiquidCrystal &displayName, machineStates machineState, vcModeStates vcState, acModeStates acState,
    float measuredPIP, float measuredPlateau, const int LCD_MAX_STRING, UserParameter *userParameters);

void displayAlarmParameters(SelectedParameter &currentlySelectedParameter, LiquidCrystal &displayName,UserParameter *userParamters);

VentilatorState setStateParameters(VentilatorState &state, UserParameter *userParameters);

void parameterSetISR();

#endif
