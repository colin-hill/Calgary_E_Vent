#define SERIAL_DEBUG //Comment this out if not debugging, used for visual confirmation of state changes
#define NO_INPUT_DEBUG //Comment this out if not debugging, used to spoof input parameters at startup when no controls are present

#include <LiquidCrystal.h>
#include "Wire.h"

#include "adc.h"
#include "elapsedMillis.h"
#include "pressure.h"
#include "alarms.h"
#include "ACMode.h"
#include "VCMode.h"
#include "breathing.h"
#include "conversions.h"
#include "MachineStates.h"
#include "Encoder.h"
#include "UserParameter.h"
#include "updateUserParameters.h"


//Begin User Defined Section----------------------------------------------------

//#define SERIAL_DEBUG //Comment this out if not debugging, used for visual confirmation of state changes
//#define NO_INPUT_DEBUG //Comment this out if not debugging, used to spoof input parameters at startup when no controls are present

const char softwareVersion[] = "Version 1.0"; //In case we test a few versions?

//IO Pin Definintions-----------------------------------------------------------
const int setParameterPin  = 25; //Pin for the set parameter button
const int limitSwitchPin   = 23;
const int alarmSwitchPin   = 24;
const int modeSwitchPin    = 20;
const int setBPMPotPin     = 7;
const int setIERatioPotPin = 8;
const int setTVPotPin      = 6;
//------------------------------------------------------------------------------

//LCD Denfinitions--------------------------------------------------------------
const int lcdEnable = 7;
const int lcdRS     = 8;
const int lcdDB4    = 9;
const int lcdDB5    = 10;
const int lcdDB6    = 11;
const int lcdDB7    = 12;

//LiquidCrystal lcd(lcdRS, lcdEnable, lcdDB4, lcdDB5, lcdDB6, lcdDB7);

//------------------------------------------------------------------------------

//ADC Definitions---------------------------------------------------------------
//------------------------------------------------------------------------------

//End User Defined Section------------------------------------------------------
//Useful Definitions and Macros-------------------------------------------------
#define ACMODE true
#define VCMODE false

#define MotorSerial Serial1

//Function Definitions---------------------------------------------------------------------------------------------------

float voltageToBPMConversion(float potVoltage);
float voltageToIERatioConversion(float potVoltage);
float voltageToTVConversion(float potVoltage);
void parameterChangeButtonISR();

// TODO: Nervous about these -- make sure that they are initialized.
//Global Variables-------------------------------------------------------------------------------------------------------
volatile boolean PARAMETER_SET = false;
SelectedParameter CURRENTLY_SELECTED_PARAMETER = e_None;
Encoder PARAMETER_SELECT_ENCODER(PARAMETER_ENCODER_PIN_1, PARAMETER_ENCODER_PIN_2);

UserParameter THRESHOLD_PRESSURE(MIN_THRESHOLD_PRESSURE,MAX_THRESHOLD_PRESSURE,THRESHOLD_PRESSURE_INCREMENT, THRESHOLD_PRESSURE_SELECT_PIN);
UserParameter BPM(MIN_BPM, MAX_BPM, BPM_INCREMENT, BPM_SELECT_PIN);
UserParameter IE_RATIO(MIN_IE_RATIO, MAX_IE_RATIO, IE_RATIO_INCREMENT, IE_RATIO_SELECT_PIN);
UserParameter TIDAL_VOLUME(MIN_TIDAL_VOLUME, MAX_TIDAL_VOLUME, TIDAL_VOLUME_INCREMENT, TIDAL_VOLUME_SELECT_PIN);

volatile float internalThresholdPressure; //Threshold pressure to be used on the next breath cycle
volatile float internalBPM; //Breaths per Minute to be used on next breath cycle
volatile float internalIERatio; //I:E ratio to be used on the next breath cycle
volatile float internalTV; //Tidal Volume to be used on next breath cycle

volatile float tempThresholdPressure;
volatile float tempBPM;
volatile float tempIERatio;
volatile float tempTV;

float loopThresholdPressure;
float loopBPM;
float loopIERatio;
float loopTV;

float pressure;
float tempPeakPressure;
float peakPressure;
float plateauPressure;
float peepPressure;

float singleBreathTime;
float inspirationTime;
float expirationTime;

uint16_t errors = 0;
//------------------------------------------------------------------------------

//Timer Variables--------------------------------------------------------------------------------------------------------
elapsedMillis parameterSetDebounceTimer;
elapsedMillis otherDebounceTimer;
elapsedMillis breathTimer;

// TODO: move these to alarms.h?
//------------------------------------------------------------------------------

//Ease of use conversion factor------------------------------------------------------------------------------------------

//------------------------------------------------------------------------------

//Enumerators------------------------------------------------------------------------------------------------------------
machineStates machineState = Startup;
acModeStates acModeState   = ACStart;
vcModeStates vcModeState   = VCStart;
//------------------------------------------------------------------------------

void setup() {

#ifdef SERIAL_DEBUG
    Serial.begin(9600);
#endif //SERIAL_DEBUG

    Serial.println("StartUpInitiated");


    // Pin Setup------------------------------------------------------------------------------------------------------------
    // Motor serial communications startup
    MotorSerial.begin(9600); //******

    //Parameter Input Pin Set Up
    setUpParameterSelectButtons(THRESHOLD_PRESSURE, BPM, IE_RATIO, TIDAL_VOLUME, PARAMETER_ENCODER_PUSH_BUTTON_PIN);

    // Pressure sensor input pin setup
    pinMode(PRESSURE_SENSOR_PIN, INPUT);


    // Parameter change interrupt setup

    // LCD Setup
    //lcd.begin(20, 4); //set number of columns and rows


    //LCD Display Startup Message for two seconds
    //displayStartScreen(softwareVersion);
#ifdef NO_INPUT_DEBUG //Skips parameter input section
    internalThresholdPressure = 5;
    internalBPM = 10;
    internalIERatio = 4;
    internalTV = 100;
#endif //NO_INPUT_DEBUG

    //Some kind of parameter start up code here, requires RT input?

    //LCD Display Homing Message
    //displayHomingScreen();

#ifdef SERIAL_DEBUG
    Serial.println("Homing Motor");
#endif //SERIAL_DEBUG

    //Motor Homing Sequence
#ifndef NO_INPUT_DEBUG
    while (digitalRead(limitSwitchPin) == 0) {
        //Move motor at Vhome********
    }

    while (digitalRead(limitSwitchPin) == 1) {
        //Move motor at Vzero********
    }

    //Hardcoded motor bag limit find sequence
    //Move motor x degrees inward********

    //Zero the encoder********
#endif //NO_INPUT_DEBUG

    machineState = BreathLoopStart;
}

void loop() {
    //Update LCD*********

    //Update the user input parameters
    updateUserParameters(CURRENTLY_SELECTED_PARAMETER, PARAMETER_SET, PARAMETER_SELECT_ENCODER,
                       THRESHOLD_PRESSURE, BPM, IE_RATIO, TIDAL_VOLUME); 

    //Beginning of state machine code

    if (BreathLoopStart == machineState) { //BreathLoopStart---------------------------------------------------------------------------------
#ifdef SERIAL_DEBUG
        Serial.println("Breath Loop Start");
#endif //SERIAL_DEBUG

        cli();
        loopThresholdPressure = internalThresholdPressure;
        loopBPM = internalBPM;
        loopIERatio = internalIERatio;
        loopTV = internalTV;
        sei();

        singleBreathTime = 60.0/loopBPM;
        inspirationTime = singleBreathTime / (1 + loopIERatio);
        expirationTime = singleBreathTime - inspirationTime;

        if (digitalRead(modeSwitchPin) == ACMODE) {
            machineState = ACMode;
        }
        else {
            machineState = VCMode;
        }
    }
    else if (ACMode == machineState) {
        acModeState = ac_mode_step(acModeState, breathTimer, inspirationTime, expirationTime,
                                   tempPeakPressure, peakPressure, pressure, peepPressure,
                                   plateauPressure, loopThresholdPressure, errors, machineState);
    }
    else if (VCMode == machineState) {
        vcModeState = vc_mode_step(vcModeState, breathTimer, inspirationTime, expirationTime,
                                   tempPeakPressure, peakPressure, pressure, peepPressure,
                                   plateauPressure, errors, machineState);
    }

    handle_alarms(errors);
}

//FUNCTIONS

void parameterSetISR() {

    if(parameterSetDebounceTimer > (0.25*S_TO_MS)){
        parameterSetDebounceTimer = 0;
        PARAMETER_SET = true;
    }
}
