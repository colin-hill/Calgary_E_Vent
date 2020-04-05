#include <elapsedMillis.h>
#include "LiquidCrystal.h"
#include "Wire.h"


//Begin User Defined Section----------------------------------------------------

#define SERIAL_DEBUG //Comment this out if not debugging, used for visual confirmation of state changes
#define NO_INPUT_DEBUG //Comment this out if not debugging, used to spoof input parameters at startup when no controls are present

const char softwareVersion[] = "Version 1.0"; //In case we test a few versions?

//IO Pin Definintions-----------------------------------------------------------
const int setParameterPin            = 25; //Pin for the set parameter button
const int limitSwitchPin             = 23;
const int alarmSwitchPin             = 24;
const int modeSwitchPin              = 20;
const int alarmBuzzerPin             = 21;
const int pressureSensorPin          = 10;
const int setBPMPotPin               = 7;
const int setIERatioPotPin           = 8;
const int setThresholdPressurePotPin = 9;
const int setTVPotPin                = 6;
//------------------------------------------------------------------------------

//Alarm Sound definitions-------------------------------------------------------
#define alarmSoundLength 0.5 //Seconds

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

//Pressure Sensor Definitions---------------------------------------------------
const float minPressureSensorVoltage = 0.0;
const float maxPressureSensorVoltage = 5.0; // Assumed 0-5V sensor

const float minPressure = 0.0; //Absolute psi,cmH20?
const float maxPressure = 3.0; //Absolute psi, cmH20?
//------------------------------------------------------------------------------

//Potenitometer Definitions-----------------------------------------------------
const float bpmPotMaxVoltage = 5.0;
const float ieRatioPotMaxVoltage = 5.0;
const float tvPotMaxVoltage = 5.0;
const float setThresholdPressurePotMaxVoltage = 5.0;
//------------------------------------------------------------------------------

//Breath Per Minute Definitions-------------------------------------------------
const float minBPM = 10.0; //Breaths per Minute
const float maxBPM = 40.0; //Breaths per Minute
//------------------------------------------------------------------------------

//Tidal Volume Definitions------------------------------------------------------
const float minTV = 0.0; //Tidal Volume (% of max)
const float maxTV = 100.0; //Tidal Volume (% of max)
//------------------------------------------------------------------------------

//Inspiration Expiration Ratio Definitions--------------------------------------
const float minIERatio = 1.0; //Inspiration to Expiration ratio 1:1
const float maxIERatio = 4.0; //Inspiration to Expiration ratio 1:4
//------------------------------------------------------------------------------

//Threshold Pressure Definitions------------------------------------------------
const float minThresholdPressure = 1.0;
const float maxThresholdPressure = 2.0;
//------------------------------------------------------------------------------

//Max & Min Pressures-----------------------------------------------------------
const float maxPeepPressure = 20.0; //cmH2O
const float minPeepPressure = 0.0; //cmH2O
//------------------------------------------------------------------------------

//Breath hold time--------------------------------------------------------------
const float holdTime        = 0.25; //Seconds
const float acThresholdTime = 0.5; //Seconds

//End User Defined Section------------------------------------------------------
//Useful Definitions and Macros-------------------------------------------------
#define ACMODE true
#define VCMODE false

#define MotorSerial Serial1

// Alarm flags
const uint16_t HIGH_PRESSURE_ALARM  = 0x01 << 0;
const uint16_t LOW_PRESSURE_ALARM   = 0x01 << 1;
const uint16_t HIGH_PEEP_ALARM      = 0x01 << 2;
const uint16_t LOW_PEEP_ALARM       = 0x01 << 3;
const uint16_t DISCONNECT_ALARM     = 0x01 << 4;
const uint16_t HIGH_TEMP_ALARM      = 0x01 << 5;
const uint16_t APNEA_ALARM          = 0x01 << 6;
const uint16_t DEVICE_FAILURE_ALARM = 0x01 << 7;
//------------------------------------------------------------------------------

//Function Definitions---------------------------------------------------------------------------------------------------
float readPressureSensor(uint8_t funcPressureSensorPin);
void readPotentiometers(uint8_t thresholdPressurePotPin, uint8_t bpmPotPin, uint8_t ieRatioPotPin, uint8_t tvPotPin, volatile float &thresholdPressure, volatile float &bpm, volatile float &ieRatio, volatile float &tv);
float voltageToPressureConversion(float sensorVoltage);
float voltageToSetThresholdPressureConversion(float potVoltage);
float voltageToBPMConversion(float potVoltage);
float voltageToIERatioConversion(float potVoltage);
float voltageToTVConversion(float potVoltage);
void parameterChangeButtonISR();

enum machineStates {
                    Startup,
                    MotorZeroing,
                    BreathLoopStart,
                    ACMode,
                    VCMode
};

enum acModeStates {
                   ACStart,
                   ACInhaleWait,
                   ACInhaleCommand,
                   ACInhale,
                   ACPeak,
                   ACExhale,
                   ACReset
};

enum vcModeStates {
                   VCStart,
                   VCInhale,
                   VCInhaleCommand,
                   VCPeak,
                   VCExhale,
                   VCReset
};


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
elapsedMillis alarmBuzzerTimer;
elapsedMillis highPressureAlarmTimer;
elapsedMillis lowPressureAlarmTimer;
elapsedMillis highPEEPAlarmTimer;
elapsedMillis lowPEEPAlarmTimer;
elapsedMillis disconnectAlarmTimer;
elapsedMillis highTempAlarmTimer;
elapsedMillis apneaAlarmTimer;
elapsedMillis deviceFaiulureAlarmTimer;

//------------------------------------------------------------------------------

//Ease of use conversion factor------------------------------------------------------------------------------------------
float adcReadingToVoltsFactor = maxADCVoltage / ((float)maxADCValue);
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
    MotorSerial.begin(9600); //********

    //Potentiometer input pin setup
    pinMode(setThresholdPressurePotPin , INPUT);
    pinMode(setBPMPotPin               , INPUT);
    pinMode(setIERatioPotPin           , INPUT);
    pinMode(setTVPotPin                , INPUT);

    //Switch input pin setup
    pinMode(setParameterPin, INPUT);
    pinMode(limitSwitchPin, INPUT);
    pinMode(alarmSwitchPin, INPUT);

    //Pressure sensor input pin setup
    pinMode(pressureSensorPin, INPUT);

    pinMode(alarmBuzzerPin,OUTPUT);

    //Parameter change interrupt setup
    attachInterrupt(digitalPinToInterrupt(setParameterPin),parameterChangeButtonISR,FALLING);

    //LCD Setup
    //lcd.begin(20, 4); //set number of columns and rows

    readPotentiometers(setThresholdPressurePotPin, setBPMPotPin, setIERatioPotPin, setTVPotPin, internalThresholdPressure, internalBPM, internalIERatio, internalTV);

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




        readPotentiometers(setThresholdPressurePotPin, setBPMPotPin, setIERatioPotPin, setTVPotPin, internalThresholdPressure, internalBPM, internalIERatio, internalTV);
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
    if (paramChange == true) {
        sei();
        readPotentiometers(setThresholdPressurePotPin, setBPMPotPin, setIERatioPotPin, setTVPotPin, tempThresholdPressure, tempBPM, tempIERatio, tempTV);

        //LCD display temp screen and variables
        //displayParameterScreen(tempTV, tempBPM, tempIERatio, tempThresholdPressure);

    }
    else {
        sei();

        //LCD display internal variables and regular screen
        //displayVentilationScreen(internalTV, internalBPM, internalIERatio, internalThresholdPressure, machineState, peakPressure, plateauPressure, peepPressure);
    }

    //Beginning of state machine code

    if (machineState == BreathLoopStart) { //BreathLoopStart---------------------------------------------------------------------------------
    
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
    else if (machineState == ACMode) { //Check for ACMode
        if (acModeState == 0) { //ACStart
#ifdef SERIAL_DEBUG
            Serial.println("ACStart");
#endif //SERIAL_DEBUG
            breathTimer = 0;
            //Send motor to zero point******* (Consider watchdog timer for each state)
            acModeState = ACInhaleWait;
        }
        if (acModeState == ACInhaleWait) { //ACInhaleWait-------------------------------------------------------------------------------------
      
#ifdef SERIAL_DEBUG
            Serial.print("ACInhaleWait: ");
            Serial.println(breathTimer);
#endif //SERIAL_DEBUG
      
            pressure = readPressureSensor(pressureSensorPin);

            if (breathTimer > acThresholdTime * 1000) {
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
        else if (acModeState == ACInhaleCommand) { //ACInhale Command
      
#ifdef SERIAL_DEBUG
            Serial.print("ACInhaleCommand: ");
#endif //SERIAL_DEBUG
      
            //Set motor velocity and position********
            acModeState = ACInhale;
        }//----------------------------------------------------------------------------------
        else if (acModeState == ACInhale) { //ACInhale-------------------------------------------------------------------------------------

#ifdef SERIAL_DEBUG
            Serial.print("ACInhale: ");
            Serial.println(breathTimer);
            Serial.print("Desired Inhale Time: ");
            Serial.println(inspirationTime);
#endif //SERIAL_DEBUG

            //Check motor position********

            pressure = readPressureSensor(pressureSensorPin);

            if (pressure > tempPeakPressure) { //Update the peak pressure
                tempPeakPressure = pressure;
            }

            if (breathTimer > inspirationTime * 1000) {
                breathTimer = 0;
                acModeState = ACPeak;
                peakPressure = tempPeakPressure;
                //Check that motor made it to the appropriate position********
            }
            else if (pressure > maxPressure) {
                errors |= HIGH_PRESSURE_ALARM;
            }
        }//------------------------------------------------------------------------------
        else if (acModeState == ACPeak) { //ACPeak-----------------------------------------------------------------------------------------

#ifdef SERIAL_DEBUG
            Serial.print("ACPeak: ");
            Serial.println(breathTimer);
            Serial.print("Desired Peak Time: ");
            Serial.println(holdTime);
#endif //SERIAL_DEBUG
      
            //Hold motor in position********

            pressure = readPressureSensor(pressureSensorPin);

            if (breathTimer > holdTime * 1000) { //******** how and where is hold time defined, currently hard coded
                acModeState = ACExhale;
                plateauPressure = pressure;
                breathTimer = 0;
            }
            else if (pressure > maxPressure) {
                errors |= HIGH_PRESSURE_ALARM;
            }
        }//---------------------------------------------------------------------------------
        else if (acModeState == ACExhale) { //ACExhale---------------------------------------------------------------------------------------

#ifdef SERIAL_DEBUG
            Serial.print("ACExhale: ");
            Serial.println(breathTimer);
            Serial.print("Desired Exhale Time: ");
            Serial.println(expirationTime);
#endif //SERIAL_DEBUG
      
            //Send motor to zero position********
  
            pressure = readPressureSensor(pressureSensorPin);

            if (breathTimer > expirationTime * 1000) {
                acModeState = ACReset;
                peepPressure = pressure;
            }
        }//---------------------------------------------------------------------------------
        else if (acModeState == ACReset) { //ACReset-----------------------------------------------------------------------------------------

#ifdef SERIAL_DEBUG
            Serial.print("ACReset");
#endif //SERIAL_DEBUG

            readPressureSensor(pressureSensorPin, pressure);

            if (pressure > maxPeepPressure) {
                errors |= HIGH_PEEP_ALARM;
            }
            else if (pressure < minPeepPressure) {
                errors |= LOW_PEEP_ALARM;
            }
            breathTimer = 0;

            acModeState = ACStart;
            machineState = BreathLoopStart;
        }
    }//End ACMode
    else if (machineState == VCMode) { //VCMode-------------------------------------------------------------------------------------------
        if (vcModeState == VCStart) { //VCStart-----------------------------------------------------------------------------------------------

#ifdef SERIAL_DEBUG
            Serial.print("VCStart");
#endif //SERIAL_DEBUG
      
            breathTimer = 0;
            vcModeState = VCInhaleCommand;
            tempPeakPressure = 0;
        }//----------------------------------------------------------------------------------
        else if (vcModeState == VCInhaleCommand) { //VCInhaleCommand----------------------------------------------------------------------

#ifdef SERIAL_DEBUG
            Serial.print("VCInhaleCommand");
#endif //SERIAL_DEBUG

            //Set motor speed and position
      
            vcModeState = VCInhale;
        }//---------------------------------------------------------------------------------------
        else if (vcModeState == VCInhale) { //VCInhale---------------------------------------------------------------------------------------------

#ifdef SERIAL_DEBUG
            Serial.print("VCInhale: ");
            Serial.println(breathTimer);
            Serial.print("Desired Inhale Time: ");
            Serial.println(inspirationTime);
#endif //SERIAL_DEBUG

            //Set motor position and speed
      
            readPressureSensor(pressureSensorPin,pressure);

            if (pressure > tempPeakPressure) {
                tempPeakPressure = pressure;
            }

            if(breathTimer > inspirationTime*1000){
                vcModeState = VCPeak;
                breathTimer = 0;
                peakPressure = tempPeakPressure;
                //Check motor position********
            }

            if (pressure > maxPressure) {
                errors |= HIGH_PRESSURE_ALARM;
            }
        }//--------------------------------------------------------------------------------
        else if (vcModeState == VCPeak) { //VCPeak------------------------------------------------------------------------------------------

#ifdef SERIAL_DEBUG
            Serial.print("VCPeak: ");
            Serial.println(breathTimer);
            Serial.print("Desired Peak Time: ");
            Serial.println(inspirationTime);
#endif //SERIAL_DEBUG

            //Hold motor in position********

            readPressureSensor(pressureSensorPin, pressure);

            if(breathTimer > holdTime*1000){
                vcModeState = VCExhale;
                breathTimer = 0;
                plateauPressure = pressure;
            }

            if (pressure > maxPressure) {
                errors |= HIGH_PRESSURE_ALARM;
            }
        }//-------------------------------------------------------------------------------
        else if (vcModeState == VCExhale) { //VCExhale-------------------------------------------------------------------------------------

#ifdef SERIAL_DEBUG
            Serial.print("VCExhale: ");
            Serial.println(breathTimer);
            Serial.print("Desired Exhale Time: ");
            Serial.println(expirationTime);
#endif //SERIAL_DEBUG

            //Set motor vlocity and desired position

            readPressureSensor(pressureSensorPin, pressure);

            if (breathTimer > expirationTime) {
                vcModeState = VCReset;
                peepPressure = pressure;
            }

            if (pressure > maxPeepPressure) {
                errors |= HIGH_PEEP_ALARM;
            }
            else if (pressure < minPeepPressure) {
                errors |= LOW_PEEP_ALARM;
            }
        }//--------------------------------------------------------------------------------
        else if(vcModeState == VCReset){ //VCReset-----------------------------------------------------------------------------------

#ifdef SERIAL_DEBUG
            Serial.print("VCReset");
#endif //SERIAL_DEBUG

            machineState = BreathLoopStart;
            vcModeState = VCStart;
  
        }
    }// End VCMode----------------------------------------------------------------------------------------------------------------------------
    //Error Handling-------------------------------------------------------------------------------------------------------------------
    if(errors & 0xFF){ //There is an unserviced error
        //Control the buzzer
        if(alarmBuzzerTimer > alarmSOundLength*1000){
            alarmBuzzerTimerr = 0; //Reset the timer
            digitalWrite(alarmBuzzerPin,!digitalRead(alarmBuzzerPin)); //Toggle the buzzer output pin
        }

        //Provide the appropriate screen for the error, error flags held in a 16 bit unsigned integer
        if(errors & HIGH_PRESSURE_ALARM){
            //Display high pressure alarm screen********
        }
        else if(errors & LOW_PRESSURE_ALARM){
            //Display low pressure alarm screen********
        }
        else if(errors & HIGH_PEEP_ALARM){
            //Display high PEEP alarm screen********
        }
        else if(errors & LOW_PEEP_ALARM){
            //Display low PEEP alarm screen********
        }
        else if(errors & DISCONNECT_ALARM){
            //Display disconnect alarm (also a low pressure alarm)
        }
        else if(errors & HIGH_TEMP_ALARM){
            //Display high temp alarm screen********
        }
        else if(errors & APNEA_ALARM){
            //Display the apnea alarm screen********
        }
        else if(errors & DEVICE_FAILURE_ALARM){
            //Display the device failure alarm********
        }
        else{
            errors = 0;
        }
    }
    else{
        alarmBuzzerTimer = 0;
        digitalWrite(alarmBuzzerPin,LOW);
    }
    //End error Handling-------------------------------------------------------------------------------------------






}

//FUNCTIONS

/* Function to read the pressure sensor on the pressureSensorPin.
 * Inputs: None.
 * Outputs:
 *  -The pressure is returned in a float
 *    + TODO: Units?
 */
float readPressureSensor(){
    return voltageToPressureConversion(analogRead(pressureSensorPin));
}

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

    float setThresholdPressurePotVoltage = setThresholdPressurePotPinADCReading * adcReadingToVoltsFactor;
    float setBPMPotVoltage = setBPMPotPinADCReading * adcReadingToVoltsFactor;
    float setIERatioPotVoltage = setIERatioPotPinADCReading * adcReadingToVoltsFactor;
    float setTVPotVoltage = setTVPotPinADCReading * adcReadingToVoltsFactor;

    thresholdPressure = voltageToSetThresholdPressureConversion(setThresholdPressurePotVoltage);
    bpm = voltageToBPMConversion(setBPMPotVoltage);
    ieRatio = voltageToIERatioConversion(setIERatioPotVoltage);
    tv = voltageToTVConversion(setTVPotVoltage);

    return;
}

/*Function to convert pressure sensor voltage to usable pressure value
  Inputs:
  -sensorVoltage: The voltage output by the sesnor, must be converted to volts from adc reading prior to use of function

  Outputs:
  -pressure in (psi? cmH2O)
*/
float voltageToPressureConversion(const float sensorVoltage) {
    return (maxPressure - minPressure) / (maxPressureSensorVoltage - minPressureSensorVoltage) * sensorVoltage + minPressure;
}

/*Function to convert the pressure potentiometer voltage to a desired set pressure
  Inputs:
  -potVoltage: The voltage output of the potentiometer, must be converted to volts from adc reading prior to use of function

  Outputs:
  -setPressure (in psi? cmH2O?)
*/
float voltageToSetThresholdPressureConversion(const float potVoltage) {
    return (maxThresholdPressure - minThresholdPressure) / setThresholdPressurePotMaxVoltage * potVoltage + minThresholdPressure;
}

/*Function to convert the breaths per minute potentiometer voltage to a desired breaths per minute
  * Inputs:
  *    -potVoltage: The voltage output of the potentiometer, must be converted to volts from adc reading prior to use of function
  * 
  * Outputs:
  *    -set breaths per mintue
  */
float voltageToBPMConversion(const float potVoltage){
    return (maxBPM - minBPM) / bpmPotMaxVoltage * potVoltage + minBPM;
}

/*Function to convert inspiration - expiration ratio potentiometer voltage to a desired IE ratio
  * Inputs:
  *    -potVoltage: The voltage output of the potentiometer, must be converted to volts from adc reading prior to use of function
  * 
  * Outputs:
  *    -set IE ratio
  */
float voltageToIERatioConversion(const float potVoltage){
    return (maxIERatio - minIERatio) / ieRatioPotMaxVoltage * potVoltage + minIERation;
}

/*Function to convert tidal volume percentage potentiometer voltage to a desired tidal volume percentage
  * Inputs:
  *    -potVoltage: The voltage output of the potentiometer, must be converted to volts from adc reading prior to use of function
  * 
  * Outputs:
  *    -set tidal volume percentage
  */
float voltageToTVConversion(const float potVoltage){
    return (maxTV - minTV) / tvPotMaxVoltage * potVoltage + minTV;
}

void parameterChangeButtonISR() {

    if(paramChangeDebounceTimer > 0.1*1000){
        if(paramChange == false){
            paramChange = true;
        }
        else {
            paramChange = false;
            internalThresholdPressure = tempThresholdPressure;
            internalBPM = tempBPM;
            internalIERatio  = tempIERatio;
            internalTV = tempTV;
        }
        paramChangeDebounceTimer = 0;
    }
}

//-------------------------------------------------------------------------------------
//LCD Support Functions...here be dragons...


/*Function to display the parameter change screen on the LCD
  Inputs:
  - temp values for TV, BPM, IERatio, ThresholdPressure
*/
/*
  void displayParameterScreen(float tempTV, float tempBPM, float tempIERatio, float tempThresholdPressure) {
  //Prep variable for output
  int lcdTV = roundAndCast(tempTV);
  int lcdBPM = roundAndCast(tempBPM);
  int lcdIERatio = roundAndCast(tempIERatio);
  int lcdThresholdPressure = roundAndCast(tempThresholdPressure);

  //Prep lines for output
  char parameterScreenL1[] = "PRESS SET TO CONFIRM";
  char parameterScreenL2[];
  char parameterScreenL3[];
  char parameterScreenL4[];
  sprintf(parameterScreenL2, "TV=%3d%%   BPM=%2d/MIN", lcdTV, lcdBPM); //Spacing checked
  sprintf(parameterScreenL3, "I:E=1:%1d", lcdIERatio);
  sprintf(parameterScreenL4, "AC_THRESHOLD=%2d MBAR", lcdThresholdPressure);//Spacing checked

  //Display data
  lcd.clear();
  lcd.print(parameterScreenL1);
  lcd.setCursor(0, 1);
  lcd.write(parameterScreenL2);
  lcd.setCursor(0, 2);
  lcd.write(parameterScreenL3);
  lcd.setCursor(0, 3);
  lcd.write(parameterScreenL4);
  }
*/
/*Function to display the ventilator (regular) screen on the LCD
  Inputs:
  - temp values for TV, BPM, IERatio, ThresholdPressure
  - Ventilaton mode
  - Pressure data for peak, plateau, peep
*/
/*
  void displayVentilationScreen(float intTV, float intBPM, float intIERatio, float intThresholdPressure, enum machineStates machineState, float peakPressure, float plateauPressure, float peepPressure) {

  //Prep variable for output
  int lcdTV = roundAndCast(intTV);
  int lcdBPM = roundAndCast(intBPM);
  int lcdIERatio = roundAndCast(intIERatio);
  int lcdThresholdPressure = roundAndCast(intThresholdPressure);
  int lcdPeakPressure = roundAndCast(peakPressure);
  int lcdPlateauPressure = roundAndCast(plateauPressure);
  int lcdPeepPressure = roundAndCast(peepPressure);

  char ventilatorScreenL1[];
  char ventilatorScreenL2[];
  char ventilatorScreenL3[];
  char ventilatorScreenL4[];

  //Prep first line
  if (machineState == ACMode) {
  sprintf(ventilatorScreenL1, "AC (ACT=%2d)    DATA:", lcdThresholdPressure); //Spacing checked
  }
  else if (machineState == VCMode) {
  sprintf(ventilatorScreenL1, "VC             DATA:"); //Spacing checked
  }
  else {
  sprintf(ventilatorScreenL1, "N/A        DATA:"); //Spacing checked
  }
  //Prep remaining lines
  sprintf(ventilatorScreenL2, "TV=%3d%%      PEAK:%2d", lcdTV, lcdPeakPressure); //Spacing checked
  sprintf(ventilatorScreenL3, "BPM=%2d/MIN   PLAT:%2d", lcdBPM, lcdPlateauPressure); //Spacing checked
  sprintf(ventilatorScreenL4, "I:E=%1d        PEEP=%2d", lcdIERatio, lcdPeepPressure); //Spacing checked

  //Display data
  lcd.clear();
  lcd.print(ventilatorScreenL1);
  lcd.setCursor(0, 1);
  lcd.write(ventilatorScreenL2);
  lcd.setCursor(0, 2);
  lcd.write(ventilatorScreenL3);
  lcd.setCursor(0, 3);
  lcd.write(ventilatorScreenL4);

  }
*/
/*Function to display start up screen on LCD
  Inputs:
  - current software version
*/
/*
  void displayStartScreen(const char softwareVersion[]) {
  const char splashScreen[] = "CALGARY E-VENT";
  lcd.clear();
  lcd.print(splashScreen);
  lcd.setCursor(0, 1);
  lcd.print(softwareVersion);
  delay(2000);
  }
*/
/*Function to display errors on LCD
  Inputs:
  - 8-bit error code number
  Bit   Error
  0     High Peak Pressure
  1     Low Peak Pressure/Disconnection
  2     High PEEP
  3     Low PEEP
  4     Apnea
*/
/*
  void displayErrorScreen(uint8_t error) {
  
  const char errorScreenL1[] = "ALARM CONDITIONS:";
  char errorScreenL2[]=" ";
  char errorScreenL3[]=" ";
  char errorScreenL4[]=" ";
  //Peak Pressure Alarms
  if (bitRead(error, 0) == 1) {
  sprintf(errorScreenL2, "PEAK HIGH");
  }
  if (bitRead(error, 1) == 1) {
  sprintf(errorScreenL2, "PEAK LOW/DISCONNECT");
  }
  if (bitRead(error, 2) == 1) {
  sprintf(errorScreenL3, "PEEP HIGH");
  }
  if (bitRead(error, 3) == 1) {
  sprintf(errorScreenL3, "PEEP LOW");
  }
  if (bitRead(error, 4) == 1) {
  sprintf(errorScreenL4, "APNEA");
  }
  lcd.clear();
  lcd.print(errorScreenL1);
  lcd.setCursor(0, 1);
  lcd.write(erroorScreenL2);
  lcd.setCursor(0, 2);
  lcd.write(errorScreenL3);
  lcd.setCursor(0, 3);
  lcd.write(errorScreenL4);
  }
*/
/*Function to display homing message on LCD
   
 */
/*
  void displayHomingScreen() {
  const char calibrationScreenL1[] = "Calibration in";
  const char calibrationScreenL2[] = "progress...";
  lcd.clear();
  lcd.print(calibrationScreenL1);
  lcd.setCursor(0, 1);
  lcd.print(calibrationScreenL2);
  }
*/
/*Function to display start up screen on LCD
  Inputs:
  - float

  Outputs:
  - a rounded integer
*/
/*
  int roundAndCast(float x) {
  int new_var;
  new_var = (int) round(x);
  }
*/
