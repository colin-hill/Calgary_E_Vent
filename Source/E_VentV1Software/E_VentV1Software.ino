#define SERIAL_DEBUG //Comment this out if not debugging, used for visual confirmation of state changes
//#define NO_INPUT_DEBUG //Comment this out if not debugging, used to spoof input parameters at startup when no controls are present
//#define NO_LIMIT_SWITCH_DEBUG
#define PYTHON_DEBUG //when defined: dumps key:value pairs to the serial port for logging by a seperate python script


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
#include "LED.h"
#include "FailureMode.h"
#include "MotorZeroing.h"
#include "PinAssignments.h"
#include "Motor.h"
#include "RoboClaw.h"
//Standard Arduino Library
#include "avr/wdt.h"

//Begin User Defined Section----------------------------------------------------

//Define LCD displays
LiquidCrystal alarmDisplay(ALARM_LCD_RS, ALARM_LCD_ENABLE, ALARM_LCD_DB4, ALARM_LCD_DB5, ALARM_LCD_DB6, ALARM_LCD_DB7);
LiquidCrystal ventilatorDisplay(VENT_LCD_RS, VENT_LCD_ENABLE, VENT_LCD_DB4, VENT_LCD_DB5, VENT_LCD_DB6, VENT_LCD_DB7);

//Define External Display
HardwareSerial &externalDisplay = Serial3;

//Define Motor Controller
RoboClaw motorController(&Serial2, MOTOR_CONTROLLER_TIMEOUT);

//#define SERIAL_DEBUG //Comment this out if not debugging, used for visual confirmation of state changes
//#define NO_INPUT_DEBUG //Comment this out if not debugging, used to spoof input parameters at startup when no controls are present

const char softwareVersion[] = "022521";
//------------------------------------------------------------------------------
//End User Defined Section------------------------------------------------------

//Useful Definitions and Macros------------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------------------------------------

//Function Definitions---------------------------------------------------------------------------------------------------

//Global Variables-------------------------------------------------------------------------------------------------------
volatile boolean parameterSet = false; //Flag to determine if a new set point has been selected, set by an ISR
SelectedParameter currentlySelectedParameter = e_None; //Enum to track which set point is being changed
Encoder parameterSelectEncoder(PARAMETER_ENCODER_PIN_1, PARAMETER_ENCODER_PIN_2);

float UserParameter::currentBPM = DEFAULT_BPM; //Initialize the ventilator operation variables to the power on default
float UserParameter::currentInspirationTime = DEFAULT_INSPIRATION_TIME;//these are defined for convenience
float UserParameter::currentTidalVolume = DEFAULT_TIDAL_VOLUME;

UserParameter userParameters[NUM_USER_PARAMETERS] =
   { UserParameter(MIN_THRESHOLD_PRESSURE, MAX_THRESHOLD_PRESSURE, THRESHOLD_PRESSURE_INCREMENT, THRESHOLD_PRESSURE_SELECT_PIN, DEFAULT_THRESHOLD_PRESSURE, e_ThresholdPressure),
     UserParameter(MIN_BPM, MAX_BPM, BPM_INCREMENT, BPM_SELECT_PIN, DEFAULT_BPM, e_BPM),
     UserParameter(MIN_INSPIRATION_TIME, MAX_INSPIRATION_TIME, INSPIRATION_TIME_INCREMENT, INSPIRATION_TIME_SELECT_PIN, DEFAULT_INSPIRATION_TIME,e_InspirationTime),
     UserParameter(MIN_TIDAL_VOLUME, MAX_TIDAL_VOLUME, TIDAL_VOLUME_INCREMENT, TIDAL_VOLUME_SELECT_PIN,  DEFAULT_TIDAL_VOLUME,e_TidalVolume),
     UserParameter(MIN_MODE_SELECT, MAX_MODE_SELECT, MODE_SELECT_INCREMENT, MODE_SELECT_PIN, DEFAULT_MODE_SELECT,e_ModeSelect),
     UserParameter(MIN_HIGH_PIP_ALARM, MAX_HIGH_PIP_ALARM, HIGH_PIP_ALARM_INCREMENT, HIGH_PIP_ALARM_SELECT_PIN, DEFAULT_HIGH_PIP_ALARM, e_HighPIPAlarm),
     UserParameter(MIN_LOW_PIP_ALARM, MAX_LOW_PIP_ALARM, LOW_PIP_ALARM_INCREMENT, LOW_PIP_ALARM_SELECT_PIN, DEFAULT_LOW_PIP_ALARM, e_LowPIPAlarm),
     UserParameter(MIN_HIGH_PEEP_ALARM, MAX_HIGH_PEEP_ALARM, HIGH_PEEP_ALARM_INCREMENT, HIGH_PEEP_ALARM_SELECT_PIN, DEFAULT_HIGH_PEEP_ALARM, e_HighPEEPAlarm),
     UserParameter(MIN_LOW_PEEP_ALARM, MAX_LOW_PEEP_ALARM, LOW_PEEP_ALARM_INCREMENT, LOW_PEEP_ALARM_SELECT_PIN, DEFAULT_LOW_PEEP_ALARM, e_LowPEEPAlarm),
     UserParameter(MIN_LOW_PLATEAU_PRESSURE_ALARM, MAX_LOW_PLATEAU_PRESSURE_ALARM, LOW_PLATEAU_PRESSURE_ALARM_INCREMENT, LOW_PLATEAU_PRESSURE_ALARM_SELECT_PIN, DEFAULT_LOW_PLATEAU_PRESSURE_ALARM, e_LowPlateauPressureAlarm), 
     UserParameter(MIN_RESPIRATORY_RATE_ALARM, MAX_RESPIRATORY_RATE_ALARM, HIGH_RESPIRATORY_RATE_ALARM_INCREMENT, HIGH_RESPIRATORY_RATE_ALARM_SELECT_PIN, DEFAULT_HIGH_RESPIRATORY_RATE_ALARM, e_HighRespiratoryRateAlarm)};

volatile boolean alarmReset = false; //Flag to determine if alarms have been reset, set by an ISR

// TODO: These are never set?
// TODO: Do these really have to be globals?
float measuredPIP;
float measuredPlateau;
float singleBreathTime;

float inspirationTime;
float expirationTime;
float motorReturnTime;


// Timer Variables--------------------------------------------------------------------------------------------------------
elapsedMillis parameterSetDebounceTimer;
elapsedMillis alarmResetDebounceTimer;
elapsedMillis alarmSilenceTimer;

elapsedMillis externalDisplayTimer;

// State for the ventilator.
VentilatorState state;


void setup() {
  
  wdt_disable();

  pinMode(A2, OUTPUT);
  digitalWrite(A2, HIGH);
  
  
#ifdef SERIAL_DEBUG
    Serial.begin(9600);
    Serial.println(F("Initialization starting..."));
#endif //SERIAL_DEBUG

    setupLimitSwitch();
    setUpAlarmPins();
    setUpPressureSensor();
    setupLEDPins();

    //Initialize millisecond timers to 0
    externalDisplayTimer = 0;
    alarmSilenceTimer = 0;

    //Parameter Input Pin Set Up
    setUpParameterSelectButtons(userParameters, NUM_USER_PARAMETERS, PARAMETER_ENCODER_PUSH_BUTTON_PIN);

    //Mode Switch Pin input setup
    pinMode(MODE_SWITCH_PIN, INPUT_PULLUP);

    //Recal pin
    pinMode(RECALIBRATION_PIN, INPUT_PULLUP);

    //LCD Setup
    alarmDisplay.begin(LCD_COLUMNS, LCD_ROWS);
    ventilatorDisplay.begin(LCD_COLUMNS, LCD_ROWS);

    //External Display Setup
    //Setup external display
    externalDisplay.begin(9600);
    externalDisplay.write('v'); //Clears the display
    externalDisplay.write("ALTA");

    //LCD Display Startup Message for two seconds
    digitalWrite(ALARM_BACKLIGHT, HIGH);
    digitalWrite(VENT_BACKLIGHT, HIGH);
    displayAEVStartupScreen(ventilatorDisplay, softwareVersion, LCD_MAX_STRING);
    displayAEVStartupScreen(alarmDisplay, softwareVersion, LCD_MAX_STRING);

    //Motor Controller Start
    motorController.begin(38400);
    
    // Set up ventilator state.
    state = get_init_state();
    state.machine_state = StartupHold;



#ifndef NO_LIMIT_SWITCH_DEBUG
    state.machine_state = MotorZeroing;
#endif

#ifdef NO_LIMIT_SWITCH_DEBUG
    state.machine_state = BreathLoopStart;
    motorController.SetEncM1(MOTOR_ADDRESS, 0);
#endif //Set the startup position as zero

    delay(2000);

  wdt_enable(WDTO_500MS);//Set watchdog to half a second
}


void loop() {
  
    //Reset the watchdog timer
    wdt_reset();

    //Handle Mode and Inhale/Exhale LEDs
    control_mode_LEDs(state);
    control_inhale_exhale_LEDs(state);

    //Issue motor commands
    handle_motor(motorController, state);

    //Update the state user input parameters
    updateStateUserParameters(state, currentlySelectedParameter, parameterSet, parameterSelectEncoder,
	userParameters, NUM_USER_PARAMETERS);

    //LCD display internal variables and regular screen
    if (MotorZeroing == state.machine_state) {
        displayHomingScreen(ventilatorDisplay);
    }
    else {
        displayUserParameters(currentlySelectedParameter, ventilatorDisplay, state.machine_state, state.vc_state, state.ac_state, state.peak_pressure, state.peep_pressure, state.pressure, LCD_MAX_STRING, userParameters);
    }

    //Update state internal variables
    update_state(state);

    //Beginning of state machine code
    if (MotorZeroing == state.machine_state){
        motor_zeroing_step(state);
    }
    else if (BreathLoopStart == state.machine_state) { // BreathLoopStart


        reset_alarm_capture(state);

        if(userParameters[e_ModeSelect].value < 0){
            state.machine_state = VCMode;
        }
        else{
            state.machine_state = ACMode;
        }

        setStateParameters(state, userParameters); //Must be before update_motor_settings
        update_motor_settings(state);

        check_recalibration_button(state);

    }

    else if (ACMode == state.machine_state) {
        ac_mode_step(state, userParameters, motorController);

        if (ACInhaleCommand == state.ac_state){
            state.breath_counter += 1;

            if(state.breath_counter > 0){
                calculate_respiratory_rate(state);
            }

        state.errors |= check_respiratory_rate(state, userParameters);
        }
    }
    else if (VCMode == state.machine_state) {
        vc_mode_step(state, userParameters, motorController);
    }
    else if (FailureMode == state.machine_state) {
        failure_mode(state);
    }


    loop_alarm_manager(externalDisplayTimer, alarmSilenceTimer, alarmReset, alarmDisplay, ventilatorDisplay, externalDisplay, state, userParameters, currentlySelectedParameter);

}

//FUNCTIONS

void parameterSetISR() {
    if (parameterSetDebounceTimer > (0.25*S_TO_MS)) {
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
