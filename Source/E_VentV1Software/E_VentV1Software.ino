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

//Begin User Defined Section----------------------------------------------------

//#define SERIAL_DEBUG //Comment this out if not debugging, used for visual confirmation of state changes
//#define NO_INPUT_DEBUG //Comment this out if not debugging, used to spoof input parameters at startup when no controls are present

const char softwareVersion[] = "VERSION 0.1";

//Pin Definition
const uint8_t modeSwitchPin = 40;

//LCD Denfinitions--------------------------------------------------------------

//TDOD: Assign real pins
//const int alarmLCDEnable = 11;
//const int alarmLCDRS     = 12;
//const int alarmLCDDB4    = 5;
//const int alarmLCDDB5    = 4;
//const int alarmLCDDB6    = 3;
//const int alarmLCDDB7    = 2;

const int ventilatorLCDEnable = 11;
const int ventilatorLCDRS     = 12;
const int ventilatorLCDDB4    = 5;
const int ventilatorLCDDB5    = 4;
const int ventilatorLCDDB6    = 3;
const int ventilatorLCDDB7    = 2;

//LiquidCrystal alarmDisplay(alarmLCDRS, alarmLCDEnable, alarmLCDDB4, alarmLCDDB5, alarmLCDDB6, alarmLCDDB7);
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


/*
float loopThresholdPressure;
float loopBPM;
float loopInspirationTime;
float loopTV;

float pressure;
float tempPeakPressure;
float peakPressure;

float peepPressure;
float plateauPressure;
*/

// TODO: These are never set?
// TODO: Do these really have to be globals?
float measuredPIP;
float measuredPlateau;
float singleBreathTime;

// TODO: Make these static variables in loop instead?
float inspiration_time;
float expiration_time;

//------------------------------------------------------------------------------

//Timer Variables--------------------------------------------------------------------------------------------------------
elapsedMillis parameterSetDebounceTimer;
elapsedMillis breathTimer;

VentilatorState state;

void setup() {

#ifdef SERIAL_DEBUG
   Serial.begin(9600);
   Serial.println("StartUpInitiated");
#endif //SERIAL_DEBUG

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

    //LCD Display Homing Message
    //displayHomingScreen();

#ifdef SERIAL_DEBUG
    Serial.println("Homing Motor");
#endif //SERIAL_DEBUG

    //Motor Homing Sequence
#ifndef NO_INPUT_DEBUG
    //while (digitalRead(limitSwitchPin) == 0) {
        //Move motor at Vhome********
    //}

    //while (digitalRead(limitSwitchPin) == 1) {
        //Move motor at Vzero********
    //}

    //Hardcoded motor bag limit find sequence
    //Move motor x degrees inward********

    //Zero the encoder********
#endif //NO_INPUT_DEBUG

    // Set up ventilator state.
    state = get_init_state();
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
    displayUserParameters(CURRENTLY_SELECTED_PARAMETER, ventilatorDisplay, state.machine_state, state.vc_state, state.ac_state, measuredPIP, measuredPlateau, LCD_MAX_STRING, USER_PARAMETERS);

    // Read in values for state
    update_state(state);

    //Beginning of state machine code
    // TODO: factor out into a function and turn into switch statement.
    if (BreathLoopStart == state.machine_state) { // BreathLoopStart
#ifdef SERIAL_DEBUG
        Serial.println("Breath Loop Start");
#endif //SERIAL_DEBUG

        state.loop_threshold_pressure = USER_PARAMETERS[0].value;
        // TODO: Some of these are not used.
        float loopBPM = USER_PARAMETERS[1].value;
        float loopInspirationTime = USER_PARAMETERS[2].value;
        float loopTV = USER_PARAMETERS[4].value;

        // TODO: unused?
        singleBreathTime = 60.0/loopBPM; //Hardcoded for testing

        inspiration_time = loopInspirationTime;
        expiration_time = singleBreathTime - inspiration_time;

        if (digitalRead(modeSwitchPin) == ACMODE) {
            state.machine_state = ACMode;
        }
        else {
            state.machine_state = VCMode;
        }
    }
    else if (ACMode == state.machine_state) {
        state.ac_state = ac_mode_step(state.ac_state, breathTimer, inspiration_time, expiration_time,
                                   state.temp_peak_pressure, state.peak_pressure, state.pressure,
                                   state.peep_pressure, state.plateau_pressure,
                                   state.loop_threshold_pressure, state.errors, state.machine_state);
    }
    else if (VCMode == state.machine_state) {
        state = vc_mode_step(state, inspiration_time, expiration_time);
    }

    // TODO: Move motor

    handle_alarms(state.errors);
}

//FUNCTIONS

void parameterSetISR() {

    if(parameterSetDebounceTimer > (0.25*S_TO_MS)){
        parameterSetDebounceTimer = 0;
        PARAMETER_SET = true;
    }
}
