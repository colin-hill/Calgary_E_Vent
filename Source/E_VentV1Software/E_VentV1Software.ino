#define SERIAL_DEBUG //Comment this out if not debugging, used for visual confirmation of state changes
#define NO_INPUT_DEBUG //Comment this out if not debugging, used to spoof input parameters at startup when no controls are present
//#define NO_LIMIT_SWITCH_DEBUG

#include <LiquidCrystal.h>

#include "adc.h"
#include "elapsedMillis.h"
#include "pressure.h"
#include "alarms.h"
#include "ACMode.h"
#include "VCMode.h"
#include "breathing.h"
#include "conversions.h"
#include "MachineStates.h"

#include "src/Encoder/Encoder.h"
#include "UserParameter.h"
#include "updateUserParameters.h"
#include "LCD.h"
#include "FailureMode.h"
#include "MotorZeroing.h"
#include "PinAssignments.h"
#include "Motor.h"
#include "RoboClaw.h"

//Begin User Defined Section----------------------------------------------------

//Define LCD displays
LiquidCrystal alarmDisplay(ALARM_LCD_RS, ALARM_LCD_ENABLE, ALARM_LCD_DB4, ALARM_LCD_DB5, ALARM_LCD_DB6, ALARM_LCD_DB7);
LiquidCrystal ventilatorDisplay(VENT_LCD_RS, VENT_LCD_ENABLE, VENT_LCD_DB4, VENT_LCD_DB5, VENT_LCD_DB6, VENT_LCD_DB7);

//Define Motor Controller
RoboClaw motorController(&Serial2, MOTOR_CONTROLLER_TIMEOUT);

//#define SERIAL_DEBUG //Comment this out if not debugging, used for visual confirmation of state changes
//#define NO_INPUT_DEBUG //Comment this out if not debugging, used to spoof input parameters at startup when no controls are present

const char softwareVersion[] = "VERSION 0.1";

//------------------------------------------------------------------------------

//End User Defined Section------------------------------------------------------
//Useful Definitions and Macros-------------------------------------------------


//#define MotorSerial Serial1

//Function Definitions---------------------------------------------------------------------------------------------------

// TODO: Nervous about these -- make sure that they are initialized.
//Global Variables-------------------------------------------------------------------------------------------------------
volatile boolean parameterSet = false;
SelectedParameter currentlySelectedParameter = e_None;
Encoder parameterSelectEncoder(PARAMETER_ENCODER_PIN_1, PARAMETER_ENCODER_PIN_2);

float UserParameter::currentBPM = DEFAULT_BPM; //Declaration of user parameter static variables for dynamic changing allowable bpm, inspiration time,
float UserParameter::currentInspirationTime = DEFAULT_INSPIRATION_TIME; // and plateau pause time
float UserParameter::currentPlateauPauseTime = DEFAULT_PLATEAU_PAUSE_TIME;

UserParameter userParameters[NUM_USER_PARAMETERS] = {UserParameter(MIN_THRESHOLD_PRESSURE, MAX_THRESHOLD_PRESSURE, THRESHOLD_PRESSURE_INCREMENT, THRESHOLD_PRESSURE_SELECT_PIN, DEFAULT_THRESHOLD_PRESSURE, e_ThresholdPressure),
                                                      UserParameter(MIN_BPM, MAX_BPM, BPM_INCREMENT, BPM_SELECT_PIN, BPM_DEFAULT, e_BPM),
                                                      UserParameter(MIN_INSPIRATION_TIME, MAX_INSPIRATION_TIME, INSPIRATION_TIME_INCREMENT, INSPIRATION_TIME_SELECT_PIN, DEFAULT_INSPIRATION_TIME,e_InspirationTime),
                                                      UserParameter(MIN_TIDAL_VOLUME, MAX_TIDAL_VOLUME, TIDAL_VOLUME_INCREMENT, TIDAL_VOLUME_SELECT_PIN, TIDAL_VOLUME_DEFAULT,e_TidalVolume),
                                                      UserParameter(MIN_PLATEAU_PAUSE_TIME, MAX_PLATEAU_PAUSE_TIME, PLATEAU_PAUSE_TIME_INCREMENT, PLATEAU_PAUSE_TIME_SELECT_PIN, DEFAULT_PLATEAU_PAUSE_TIME,e_PlateauPauseTime),
                                                      UserParameter(MIN_HIGH_PIP_ALARM, MAX_HIGH_PIP_ALARM, HIGH_PIP_ALARM_INCREMENT, HIGH_PIP_ALARM_SELECT_PIN, HIGH_PIP_ALARM_DEFAULT, e_HighPIPAlarm),
                                                      UserParameter(MIN_LOW_PIP_ALARM, MAX_LOW_PIP_ALARM, LOW_PIP_ALARM_INCREMENT, LOW_PIP_ALARM_SELECT_PIN, LOW_PIP_ALARM_DEFAULT, e_LowPIPAlarm),
                                                      UserParameter(MIN_HIGH_PEEP_ALARM, MAX_HIGH_PEEP_ALARM, HIGH_PEEP_ALARM_INCREMENT, HIGH_PEEP_ALARM_SELECT_PIN, HIGH_PEEP_ALARM_DEFAULT, e_HighPEEPAlarm),
                                                      UserParameter(MIN_LOW_PEEP_ALARM, MAX_LOW_PEEP_ALARM, LOW_PEEP_ALARM_INCREMENT, LOW_PEEP_ALARM_SELECT_PIN, LOW_PEEP_ALARM_DEFAULT, e_LowPEEPAlarm),
                                                      UserParameter(MIN_LOW_PLATEAU_PRESSURE_ALARM, MAX_LOW_PLATEAU_PRESSURE_ALARM, LOW_PLATEAU_PRESSURE_ALARM_INCREMENT, LOW_PLATEAU_PRESSURE_ALARM_SELECT_PIN, LOW_PLATEAU_PRESSURE_ALARM_DEFAULT, e_LowPlateauPressureAlarm)};

volatile boolean alarmReset = false;
// TODO: These are never set?
// TODO: Do these really have to be globals?
float measuredPIP;
float measuredPlateau;
float singleBreathTime;

float inspirationTime;
float expirationTime;
float motorReturnTime;



//------------------------------------------------------------------------------

//Timer Variables--------------------------------------------------------------------------------------------------------
elapsedMillis parameterSetDebounceTimer;
elapsedMillis alarmResetDebounceTimer;
elapsedMillis breathTimer;

//Enumerators------------------------------------------------------------------------------------------------------------
machineStates machineState = Startup;
zeroingStates zeroingState = CommandHome;
acModeStates acModeState   = ACStart;
vcModeStates vcModeState   = VCStart;
//------------------------------------------------------------------------------

VentilatorState state;


void setup() {

#ifdef SERIAL_DEBUG
    Serial.begin(9600);
    Serial.println("Initialization starting...");
#endif //SERIAL_DEBUG

    setupLimitSwitch();
    setUpAlarmSwitch();
    setUpPressureSensor();

    // Motor serial communications startup
    // MotorSerial.begin(9600); //********

    //Parameter Input Pin Set Up
    setUpParameterSelectButtons(userParameters, NUM_USER_PARAMETERS, PARAMETER_ENCODER_PUSH_BUTTON_PIN);

    //Mode Switch Pin input setup
    pinMode(MODE_SWITCH_PIN, INPUT_PULLUP);

    //LCD Setup
    alarmDisplay.begin(LCD_COLUMNS, LCD_ROWS);
    ventilatorDisplay.begin(LCD_COLUMNS, LCD_ROWS);

    //Motor Controller Start
    motorController.begin(38400);

    //LCD Display Startup Message for two seconds
    displayStartupScreen(ventilatorDisplay, softwareVersion, LCD_COLUMNS);

// Set up ventilator state.
    state = get_init_state();
    state.machine_state = StartupHold;

    //LCD Startup hold message
    displayStartupHoldScreen(ventilatorDisplay);

#ifdef SERIAL_DEBUG
    Serial.begin(9600);
    Serial.println("StartupHold");
    Serial.println(state.machine_state);
#endif //SERIAL_DEBUG


#ifndef NO_INPUT_DEBUG
    while(StartupHold == state.machine_state) {
    //Wait for interupt routine triggered by user

        //TODO: Declan add interrupt flag to change state

        delay(250);
    }
#endif //Skip hardware lockout for debug

#ifndef NO_LIMIT_SWITCH_DEBUG
    state.machine_state = MotorZeroing;
#endif

#ifdef NO_LIMIT_SWITCH_DEBUG
    state.machine_state = BreathLoopStart;
    motorController.SetEncM1(MOTOR_ADDRESS, 0);
#endif //Set the startup position as zero


}

void loop() {
    //Update LCD*********

    state = handle_motor(motorController, state);
    //state = handle_motor(motorController, state);

    //Update the state user input parameters
    state = updateStateUserParameters(state, currentlySelectedParameter, parameterSet, parameterSelectEncoder,
                       userParameters, NUM_USER_PARAMETERS);

    //LCD display internal variables and regular screen
    //displayVentilationScreen(internalTV, internalBPM, internalIERatio, internalThresholdPressure, machineState, peakPressure, plateauPressure, peepPressure);
    if (MotorZeroing == state.machine_state) {

        displayHomingScreen(ventilatorDisplay);
        //displayHomingScreen(alarmDisplay);
    }
    else {

        displayUserParameters(currentlySelectedParameter, ventilatorDisplay, state.machine_state, state.vc_state, state.ac_state, state.peep_pressure, state.pressure, LCD_MAX_STRING, userParameters);
    }
    //TODO: Add in alarm display
    update_motor_settings(state);

    // Read in values for state
    update_state(state);

    //Beginning of state machine code

    // TODO: factor out into a function and turn into switch statement.
    if (MotorZeroing == state.machine_state){
        state = motor_zeroing_step(state);

    }
    else if (BreathLoopStart == state.machine_state) { // BreathLoopStart

#ifdef SERIAL_DEBUG
        Serial.println("Breath Loop Start");
#endif //SERIAL_DEBUG

        state.machine_state = check_mode();
    }

    else if (ACMode == state.machine_state) {
        state = ac_mode_step(state);
    }
    else if (VCMode == state.machine_state) {
        state = vc_mode_step(state);
    }
    else if (FailureMode == state.machine_state) {
        state = failure_mode(state);
    }


    

    state = handle_alarms(alarmReset, state, alarmDisplay, userParameters, currentlySelectedParameter);

    //delay(1000);
}

//FUNCTIONS

void parameterSetISR() {

    if(parameterSetDebounceTimer > (0.25*S_TO_MS)){
        parameterSetDebounceTimer = 0;
        parameterSet = true;
    }
}

void alarmResetISR(){

  if(alarmResetDebounceTimer > (0.25*S_TO_MS)){
    alarmResetDebounceTimer = 0;
    alarmReset = true;
  }
}
