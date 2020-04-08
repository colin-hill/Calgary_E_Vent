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
#include "LCD.h"
#include "FailureMode.h"
#include "MotorZeroing.h"

//Begin User Defined Section----------------------------------------------------

//#define SERIAL_DEBUG //Comment this out if not debugging, used for visual confirmation of state changes
//#define NO_INPUT_DEBUG //Comment this out if not debugging, used to spoof input parameters at startup when no controls are present

const char softwareVersion[] = "VERSION 0.1";

//IO Pin Definintions-----------------------------------------------------------


const int alarmSwitchPin   = 24;
const int modeSwitchPin    = 40;


//LCD Denfinitions--------------------------------------------------------------

const int alarmLCDEnable = 49;
const int alarmLCDRS     = 53;
const int alarmLCDDB4    = 47;
const int alarmLCDDB5    = 45;
const int alarmLCDDB6    = 43;
const int alarmLCDDB7    = 41;

const int ventilatorLCDEnable = 51;
const int ventilatorLCDRS     = 53;
const int ventilatorLCDDB4    = 47;
const int ventilatorLCDDB5    = 45;
const int ventilatorLCDDB6    = 43;
const int ventilatorLCDDB7    = 41;

LiquidCrystal alarmDisplay(alarmLCDRS, alarmLCDEnable, alarmLCDDB4, alarmLCDDB5, alarmLCDDB6, alarmLCDDB7);
LiquidCrystal ventilatorDisplay(ventilatorLCDRS, ventilatorLCDEnable, ventilatorLCDDB4, ventilatorLCDDB5, ventilatorLCDDB6, ventilatorLCDDB7);

//------------------------------------------------------------------------------

//End User Defined Section------------------------------------------------------
//Useful Definitions and Macros-------------------------------------------------
#define ACMODE true
#define VCMODE false

//#define MotorSerial Serial1

//Function Definitions---------------------------------------------------------------------------------------------------

// TODO: Nervous about these -- make sure that they are initialized.
//Global Variables-------------------------------------------------------------------------------------------------------
volatile boolean PARAMETER_SET = false;
SelectedParameter CURRENTLY_SELECTED_PARAMETER = e_None;
Encoder PARAMETER_SELECT_ENCODER(PARAMETER_ENCODER_PIN_1, PARAMETER_ENCODER_PIN_2);

UserParameter USER_PARAMETERS[NUM_USER_PARAMETERS] = {UserParameter(MIN_THRESHOLD_PRESSURE, MAX_THRESHOLD_PRESSURE, THRESHOLD_PRESSURE_INCREMENT, THRESHOLD_PRESSURE_SELECT_PIN),
                                                      UserParameter(MIN_BPM, MAX_BPM, BPM_INCREMENT, BPM_SELECT_PIN),
                                                      UserParameter(MIN_INSPIRATION_TIME, MAX_INSPIRATION_TIME, INSPIRATION_TIME_INCREMENT, INSPIRATION_TIME_SELECT_PIN),
                                                      UserParameter(MIN_TIDAL_VOLUME, MAX_TIDAL_VOLUME, TIDAL_VOLUME_INCREMENT, TIDAL_VOLUME_SELECT_PIN),
                                                      UserParameter(MIN_PLATEAU_PAUSE_TIME, MAX_PLATEAU_PAUSE_TIME, PLATEAU_PAUSE_TIME_INCREMENT, PLATEAU_PAUSE_TIME_SELECT_PIN),
                                                      UserParameter(MIN_HIGH_PIP_ALARM, MAX_HIGH_PIP_ALARM, HIGH_PIP_ALARM_INCREMENT, HIGH_PIP_ALARM_SELECT_PIN),
                                                      UserParameter(MIN_LOW_PIP_ALARM, MAX_LOW_PIP_ALARM, LOW_PIP_ALARM_INCREMENT, LOW_PIP_ALARM_SELECT_PIN),
                                                      UserParameter(MIN_HIGH_PEEP_ALARM, MAX_HIGH_PEEP_ALARM, HIGH_PEEP_ALARM_INCREMENT, HIGH_PEEP_ALARM_SELECT_PIN),
                                                      UserParameter(MIN_LOW_PEEP_ALARM, MAX_LOW_PEEP_ALARM, LOW_PEEP_ALARM_INCREMENT, LOW_PEEP_ALARM_SELECT_PIN),
                                                      UserParameter(MIN_LOW_PLATEAU_PRESSURE_ALARM, MAX_LOW_PLATEAU_PRESSURE_ALARM, LOW_PLATEAU_PRESSURE_ALARM_INCREMENT, LOW_PLATEAU_PRESSURE_ALARM_SELECT_PIN)};


float loopThresholdPressure;
float loopBPM;
float loopInspirationTime;
float loopTV;

float pressure;
float tempPeakPressure;
float peakPressure;
float measuredPlateau;
float peepPressure;

float controllerTemperature;

float measuredPIP; 
float plateauPressure;

float singleBreathTime;
float inspirationTime;
float expirationTime;


uint16_t errors = 0;
//------------------------------------------------------------------------------

//Timer Variables--------------------------------------------------------------------------------------------------------
elapsedMillis parameterSetDebounceTimer;
elapsedMillis breathTimer;
elapsedMillis homingTimer;

//Enumerators------------------------------------------------------------------------------------------------------------
machineStates machineState = Startup;
zeroingStates zeroingState = CommandHome;
acModeStates acModeState   = ACStart;
vcModeStates vcModeState   = VCStart;
//------------------------------------------------------------------------------

void setup() {

#ifdef SERIAL_DEBUG

    Serial.begin(9600);
    Serial.println("Startup");
#endif //SERIAL_DEBUG

    setupLimitSwitch();



    // Motor serial communications startup
    // MotorSerial.begin(9600); //********

    //Parameter Input Pin Set Up
    setUpParameterSelectButtons(USER_PARAMETERS, NUM_USER_PARAMETERS, PARAMETER_ENCODER_PUSH_BUTTON_PIN);


    //modeSwitchPin input setup
    pinMode(modeSwitchPin, INPUT);

    // Parameter change interrupt setup

    //LCD Setup
    //alarmDisplay.begin(LCD_COLUMNS, LCD_ROWS); //set number of columns and rows
    ventilatorDisplay.begin(LCD_COLUMNS, LCD_ROWS);


    
    //LCD Display Startup Message for two seconds
    displayStartupScreen(ventilatorDisplay, softwareVersion, LCD_COLUMNS);


#ifdef NO_INPUT_DEBUG //Skips parameter input section
    
#endif //NO_INPUT_DEBUG

    //Some kind of parameter start up code here, requires RT input?


#ifdef SERIAL_DEBUG
    Serial.begin(9600);
    Serial.println("StartupHold");
#endif //SERIAL_DEBUG

    machineState = StartupHold; 

    while(StartupHold == machineState) {
    //Wait for interupt routine triggered by user
        delay(250);
    }


    machineState = MotorZeroing;

    
}

void loop() {
    //Update LCD*********

    
    //Update the user input parameters
    updateUserParameters(CURRENTLY_SELECTED_PARAMETER, PARAMETER_SET, PARAMETER_SELECT_ENCODER,
                       USER_PARAMETERS, NUM_USER_PARAMETERS); 
    
    //LCD display temp screen and variables
    //displayParameterScreen(tempTV, tempBPM, tempIERatio, tempThresholdPressure);


    //LCD display internal variables and regular screen
    //displayVentilationScreen(internalTV, internalBPM, internalIERatio, internalThresholdPressure, machineState, peakPressure, plateauPressure, peepPressure);
    displayUserParameters(CURRENTLY_SELECTED_PARAMETER, ventilatorDisplay, machineState, vcModeState, acModeState, measuredPIP, measuredPlateau, LCD_MAX_STRING, USER_PARAMETERS);

    //Beginning of state machine code

    

    if (MotorZeroing == machineState) {

#ifdef SERIAL_DEBUG
    Serial.println("Homing Motor");
#endif //SERIAL_DEBUG

        displayHomingScreen(ventilatorDisplay);
        displayHomingScreen(alarmDisplay);

        zeroingState = motor_zeroing_step(zeroingState, homingTimer, errors, machineState);;
    }
    else if (BreathLoopStart == machineState) { //BreathLoopStart---------------------------------------------------------------------------------
#ifdef SERIAL_DEBUG
        Serial.println("Breath Loop Start");
#endif //SERIAL_DEBUG

        loopThresholdPressure = USER_PARAMETERS[0].value; //TODO: not hardcoded numbers here
        loopBPM = USER_PARAMETERS[1].value;
        loopInspirationTime = USER_PARAMETERS[2].value;
        loopTV = USER_PARAMETERS[4].value;

        singleBreathTime = 60.0/4.0; //Hardcoded for testing
        inspirationTime = loopInspirationTime;
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
    else if (FailureMode == machineState) {
        failure_mode(errors);
    }

    //TODO Define controller temperature
    machineState = handle_alarms(machineState, alarmDisplay, errors, peakPressure, peepPressure, controllerTemperature);
    
}

//FUNCTIONS

void parameterSetISR() {

    if(parameterSetDebounceTimer > (0.25*S_TO_MS)){
        parameterSetDebounceTimer = 0;
        PARAMETER_SET = true;
    }
}
