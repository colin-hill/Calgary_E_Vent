#define SERIAL_DEBUG //Comment this out if not debugging, used for visual confirmation of state changes
#define NO_INPUT_DEBUG //Comment this out if not debugging, used to spoof input parameters at startup when no controls are present

#include <LiquidCrystal.h>
#include "Wire.h"

#include "elapsedMillis.h"
#include "pressure.h"
#include "alarms.h"
#include "ACMode.h"
#include "VCMode.h"
#include "breathing.h"
#include "conversions.h"
#include "MachineStates.h"

//Begin User Defined Section----------------------------------------------------

//#define SERIAL_DEBUG //Comment this out if not debugging, used for visual confirmation of state changes
//#define NO_INPUT_DEBUG //Comment this out if not debugging, used to spoof input parameters at startup when no controls are present

const char softwareVersion[] = "VERSION 0.1";

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
const float maxADCVoltage = 5.0;  //ATMega standard max input voltage
const int maxADCValue     = 1024; //ATMega standard 10-bit ADC
//------------------------------------------------------------------------------

//End User Defined Section------------------------------------------------------
//Useful Definitions and Macros-------------------------------------------------
#define ACMODE true
#define VCMODE false

//#define MotorSerial Serial1

//Function Definitions---------------------------------------------------------------------------------------------------
void readPotentiometers(uint8_t thresholdPressurePotPin, uint8_t bpmPotPin, uint8_t ieRatioPotPin, uint8_t tvPotPin, volatile float &thresholdPressure, volatile float &bpm, volatile float &ieRatio, volatile float &tv);

float voltageToBPMConversion(float potVoltage);
float voltageToIERatioConversion(float potVoltage);
float voltageToTVConversion(float potVoltage);
void parameterChangeButtonISR();

// TODO: Nervous about these -- make sure that they are initialized.
//Global Variables-------------------------------------------------------------------------------------------------------
volatile boolean paramChange = false;
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
elapsedMillis paramChangeDebounceTimer;
elapsedMillis otherDebounceTimer;
elapsedMillis breathTimer;

// TODO: move these to alarms.h?
//------------------------------------------------------------------------------

//Ease of use conversion factor------------------------------------------------------------------------------------------
const float ADC_READING_TO_VOLTS_FACTOR = maxADCVoltage / ((float)maxADCValue);
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


    //Pin Setup------------------------------------------------------------------------------------------------------------
    //Motor serial communications startup
//    MotorSerial.begin(9600); //********

    //Potentiometer input pin setup
    pinMode(SET_THRESHOLD_PRESSURE_POT_PIN , INPUT);
    pinMode(setBPMPotPin               , INPUT);
    pinMode(setIERatioPotPin           , INPUT);
    pinMode(setTVPotPin                , INPUT);

    //Switch input pin setup
    pinMode(setParameterPin, INPUT);
    pinMode(limitSwitchPin, INPUT);
    pinMode(alarmSwitchPin, INPUT);

    //Pressure sensor input pin setup
    pinMode(PRESSURE_SENSOR_PIN, INPUT);

    pinMode(ALARM_BUZZER_PIN,OUTPUT);

    //Parameter change interrupt setup
    attachInterrupt(digitalPinToInterrupt(setParameterPin),parameterChangeButtonISR,FALLING);

    //LCD Setup
    //lcd.begin(20, 4); //set number of columns and rows

    readPotentiometers(SET_THRESHOLD_PRESSURE_POT_PIN, setBPMPotPin, setIERatioPotPin, setTVPotPin, internalThresholdPressure, internalBPM, internalIERatio, internalTV);

    //LCD Display Startup Message for two seconds
    //displayStartScreen(softwareVersion);
#ifdef NO_INPUT_DEBUG //Skips parameter input section
    cli();
    paramChange = true;
    sei();
    internalThresholdPressure = 5;
    internalBPM = 10;
    internalIERatio = 4;
    internalTV = 100;
#endif //NO_INPUT_DEBUG
    
    cli(); //Turn off ointerrupts before reading paramChange
    while (paramChange == false) {

        sei();
        //LCD Display Internal Variables
        //displayParameterScreen(internalTV, internalBPM, internalIERatio, internalThresholdPressure);




        readPotentiometers(SET_THRESHOLD_PRESSURE_POT_PIN, setBPMPotPin, setIERatioPotPin, setTVPotPin, internalThresholdPressure, internalBPM, internalIERatio, internalTV);
        cli();
    }
    paramChange = false;
    sei();

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
    cli(); //Prevent interrupts from occuring
    if (paramChange) {
        sei();
        readPotentiometers(SET_THRESHOLD_PRESSURE_POT_PIN, setBPMPotPin, setIERatioPotPin, setTVPotPin, tempThresholdPressure, tempBPM, tempIERatio, tempTV);

        //LCD display temp screen and variables
        //displayParameterScreen(tempTV, tempBPM, tempIERatio, tempThresholdPressure);

    }
    else {
        sei();

        //LCD display internal variables and regular screen
        //displayVentilationScreen(internalTV, internalBPM, internalIERatio, internalThresholdPressure, machineState, peakPressure, plateauPressure, peepPressure);
    }

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
    
        if(digitalRead(modeSwitchPin) == ACMODE){
            machineState = ACMode;
        }
        else{
            machineState = VCMode;
        }
    }//----------------------------------------------------------------------------------------------------------------------
    //ACMode-----------------------------------------------------------------------------------------------------------------
    else if (ACMode == machineState) { //Check for ACMode
        if (ACStart == acModeState) { //ACStart
#ifdef SERIAL_DEBUG
            Serial.println("ACStart");
#endif //SERIAL_DEBUG
            breathTimer = 0;
            //Send motor to zero point******* (Consider watchdog timer for each state)
            acModeState = ACInhaleWait;
        }
        if (ACInhaleWait == acModeState) { //ACInhaleWait-------------------------------------------------------------------------------------
      
#ifdef SERIAL_DEBUG
            Serial.print("ACInhaleWait: ");
            Serial.println(breathTimer);
#endif //SERIAL_DEBUG
      
            pressure = readPressureSensor();

            if (breathTimer > (AC_THRESHOLD_TIME * S_TO_MS)) {
                acModeState = ACInhaleCommand;
                errors |= APNEA_ALARM;
                breathTimer = 0;
                tempPeakPressure = 0;
            }
            else if(pressure < loopThresholdPressure){
                acModeState = ACInhaleCommand;
                breathTimer = 0;
                tempPeakPressure = 0;
            }
        }//------------------------------------------------------------------------------
        else if (ACInhaleCommand == acModeState) { //ACInhale Command
      
#ifdef SERIAL_DEBUG
            Serial.print("ACInhaleCommand: ");
#endif //SERIAL_DEBUG
      
            //Set motor velocity and position********
            acModeState = ACInhale;
        }//----------------------------------------------------------------------------------
        else if (ACInhale == acModeState) { //ACInhale-------------------------------------------------------------------------------------

#ifdef SERIAL_DEBUG
            Serial.print("ACInhale: ");
            Serial.println(breathTimer);
            Serial.print("Desired Inhale Time: ");
            Serial.println(inspirationTime);
#endif //SERIAL_DEBUG

            //Check motor position********

            pressure = readPressureSensor();

            if (pressure > tempPeakPressure) { //Update the peak pressure
                tempPeakPressure = pressure;
            }

            // TODO: nervous about this else if for alarm.
            if (breathTimer > (inspirationTime * S_TO_MS)) {
                breathTimer = 0;
                acModeState = ACPeak;
                peakPressure = tempPeakPressure;
                //Check that motor made it to the appropriate position********
            }
            else if (pressure > MAX_PRESSURE) {
                errors |= HIGH_PRESSURE_ALARM;
            }
        }//------------------------------------------------------------------------------
        else if (ACPeak == acModeState) { //ACPeak-----------------------------------------------------------------------------------------

#ifdef SERIAL_DEBUG
            Serial.print("ACPeak: ");
            Serial.println(breathTimer);
            Serial.print("Desired Peak Time: ");
            Serial.println(HOLD_TIME);
#endif //SERIAL_DEBUG
      
            //Hold motor in position********

            pressure = readPressureSensor();

            if (breathTimer > (HOLD_TIME * S_TO_MS)) { //******** how and where is hold time defined, currently hard coded
                acModeState = ACExhale;
                plateauPressure = pressure;
                breathTimer = 0;
            }
            else if (pressure > MAX_PRESSURE) {
                errors |= HIGH_PRESSURE_ALARM;
            }
        }//---------------------------------------------------------------------------------
        else if (ACExhale == acModeState) { //ACExhale---------------------------------------------------------------------------------------

#ifdef SERIAL_DEBUG
            Serial.print("ACExhale: ");
            Serial.println(breathTimer);
            Serial.print("Desired Exhale Time: ");
            Serial.println(expirationTime);
#endif //SERIAL_DEBUG
      
            //Send motor to zero position********
  
            pressure = readPressureSensor();

            if (breathTimer > (expirationTime * S_TO_MS)) {
                acModeState = ACReset;
                peepPressure = pressure;
            }
        }//---------------------------------------------------------------------------------
        else if (ACReset == acModeState) { //ACReset-----------------------------------------------------------------------------------------

#ifdef SERIAL_DEBUG
            Serial.print("ACReset");
#endif //SERIAL_DEBUG

            pressure = readPressureSensor();
            errors |= check_peep(pressure);
            breathTimer = 0;

            acModeState = ACStart;
            machineState = BreathLoopStart;
        }
    }//End ACMode
    else if (VCMode == machineState) {
        vcModeState = vc_mode_step(vcModeState, breathTimer, inspirationTime, expirationTime,
                     tempPeakPressure, peakPressure, pressure, peepPressure,
                     plateauPressure, errors, machineState);
    }
    
    handle_alarms(errors);
}

//FUNCTIONS


/*Function to read the potentiometer inputs
  * Inputs:
  *  -The Threshold pressure potentiometer pin
  *  -The BPM potentiometer pin
  *  -The IERatio potentiometer pin
  *  -The TV potentiometer pin
  *  -Pointers to the threshold pressure, BPM, IERatio and TV variables
  */
void readPotentiometers(uint8_t thresholdPressurePotPin, uint8_t bpmPotPin, uint8_t ieRatioPotPin, uint8_t tvPotPin, volatile float &thresholdPressure, volatile float &bpm, volatile float &ieRatio, volatile float &tv) {
    uint16_t setThresholdPressurePotPinADCReading = analogRead(thresholdPressurePotPin);
    uint16_t setBPMPotPinADCReading = analogRead(bpmPotPin);
    uint16_t setIERatioPotPinADCReading = analogRead(ieRatioPotPin);
    uint16_t setTVPotPinADCReading = analogRead(tvPotPin);

    float setThresholdPressurePotVoltage = setThresholdPressurePotPinADCReading * ADC_READING_TO_VOLTS_FACTOR;
    float setBPMPotVoltage = setBPMPotPinADCReading * ADC_READING_TO_VOLTS_FACTOR;
    float setIERatioPotVoltage = setIERatioPotPinADCReading * ADC_READING_TO_VOLTS_FACTOR;
    float setTVPotVoltage = setTVPotPinADCReading * ADC_READING_TO_VOLTS_FACTOR;

    thresholdPressure = voltageToSetThresholdPressureConversion(setThresholdPressurePotVoltage);
    bpm = voltageToBPMConversion(setBPMPotVoltage);
    ieRatio = voltageToIERatioConversion(setIERatioPotVoltage);
    tv = voltageToTVConversion(setTVPotVoltage);

    return;
}

void parameterChangeButtonISR() {

    if(paramChangeDebounceTimer > 0.1*1000){
        if(paramChange){
            paramChange = false;
            internalThresholdPressure = tempThresholdPressure;
            internalBPM = tempBPM;
            internalIERatio  = tempIERatio;
            internalTV = tempTV;
        }
        else {
            paramChange = true;
        }
        paramChangeDebounceTimer = 0;
    }
}
