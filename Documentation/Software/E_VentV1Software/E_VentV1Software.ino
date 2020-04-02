#include "elapsedMillis.h"
#include "LiquidCrystal.h"

//Begin User Defined Section---------------------------------------------------------------------------------------------

const char softwareVersion[] = "Version 1.0"; //In case we test a few versions?

//IO Pin Definintions----------------------------------------------------------------------------------------------------
#define setParameterPin 25 //Pin for the set parameter button
#define limitSwitchPin 23
#define alarmSwitchPin 24
#define pressureSensorPin 10
#define setBPMPotPin 7
#define setIERatioPotPin 8
#define setPressurePotPin 9
#define setTVPotPin 6
//-----------------------------------------------------------------------------------------------------------------------

//LCD Denfinitions---------------------------------------------------------------------------------------------------
#define lcdEnable 7
#define lcdRS 8
#define lcdDB4 9
#define lcdDB5 10
#define lcdDB6 11
#define lcdDB7 12

LiquidCrystal lcd(lcdRS, lcdEnable, lcdDB4, lcdDB5, lcdDB6, lcdDB7);

//----------------------------------------------------------------------------------------------------------------------

//ADC Definitions--------------------------------------------------------------------------------------------------------
#define maxADCVoltage 5.0 //ATMega standard max input voltage
#define maxADCValue 1024 //ATMega standard 10-bit ADC
//-----------------------------------------------------------------------------------------------------------------------

//Pressure Sensor Definitions--------------------------------------------------------------------------------------------
#define minPressurSensorVoltage 0.0
#define maxPressureSensorVoltage 5.0 // Assumed 0-5V sensor
#define minPressure 0.0 //Absolute psi,cmH20?
#define maxPressure 3.0 //Absolute psi, cmH20?
//-----------------------------------------------------------------------------------------------------------------------

//Potenitometer Definitions----------------------------------------------------------------------------------------------
#define bpmPotMaxVoltage 5.0
#define ieRatioPotMaxVoltage 5.0
#define tvPotMaxVoltage 5.0
#define setThresholdPressurePotMaxVoltage 5.0
//-----------------------------------------------------------------------------------------------------------------------

//Breath Per Minute Definitions------------------------------------------------------------------------------------------
#define minBPM 10.0 //Breaths per Minute
#define maxBPM 40.0 //Breaths per Minute
//-----------------------------------------------------------------------------------------------------------------------

//Tidal Volume Definitions-----------------------------------------------------------------------------------------------
#define minTV 0.0 //Tidal Volume (% of max)
#define maxTV 100.0 //Tidal Volume (% of max)
//-----------------------------------------------------------------------------------------------------------------------

//Inspiration Expiration Ratio Definitions-------------------------------------------------------------------------------
#define minIERatio 1.0 //Inspiration to Expiration ratio 1:1
#define maxIERatio 0.25 //Inspiration to Expiration ratio 1:4
//-----------------------------------------------------------------------------------------------------------------------

//Threshold Pressure Definitions-----------------------------------------------------------------------------------------
#define minThresholdPressure 1.0
#define maxThresholdPressure 2.0
//-----------------------------------------------------------------------------------------------------------------------

//Max & Min Pressures---------------------------------------------------------------------------------------------------
float maxPressure = 40.0; //cmH2O
float maxPeepPressure = 20.0; //cmH2O
float minPeepPressure = 0.0; //cmH2O
//-----------------------------------------------------------------------------------------------------------------------

//Breath hold time-------------------------------------------------------------------------------------------------------
float holdTime = 0.25; //Seconds

//End User Defined Section-----------------------------------------------------------------------------------------------
//Useful Definitions and Macros------------------------------------------------------------------------------------------
#define ACMODE true
#define VCMODE false

enum machineStates {
  Startup,
  MotorZeroing,
  BreathLoopStart,
  ACMode,
  VCMode
}

enum acModeStates {
  ACStart,
  ACInhaleWait,
  ACInhale,
  ACPeak,
  ACExhale,
  ACReset
}

enum vcModeStates {
  VCStart,
  VCInhale,
  VCPeak,
  VCExhale,
  VCReset
}

//Global Variables-------------------------------------------------------------------------------------------------------
volatile bool paramChange = false;
volatile float internalThresholdPressure; //Threshold pressure to be used on the next breath cycle
volatile float internalBPM; //Breaths per Minute to be used on next breath cycle
volatile float internalIERatio; //I:E ratio to be used on the next breath cycle
volatile float internalTV; //Tidal Volume to be used on next breath cycle

float tempThresholdPressure;
float tempBPM;
float tempIERatio;
float tempTV;

float pressure;
float tempPeakPressure;
float peakPressure;
float plateauPressure;
float peepPressure;
//-----------------------------------------------------------------------------------------------------------------------

//Timer Variables--------------------------------------------------------------------------------------------------------
volatile elapsedMillis paramChangeDebounceTimer;
volatile elapsedMillis otherDebounceTimer;
elapsedMillis breathTimer;
//-----------------------------------------------------------------------------------------------------------------------

//Ease of use conversion factor------------------------------------------------------------------------------------------
float adcReadingToVoltsFactor = (maxADCVoltage - 0.0) / (maxADCValue - 0);
//-----------------------------------------------------------------------------------------------------------------------

//Enumerators------------------------------------------------------------------------------------------------------------
machineStates machineState = Startup;
acModeStates acModeState = ACStart;
vcModeStates vcModeState = VCStart;
//-----------------------------------------------------------------------------------------------------------------------

void setup() {

  //Pin Setup------------------------------------------------------------------------------------------------------------
  //Motor serial communications startup
  Serial.begin(9600); ********

  //Potentiometer input pin setup
  pinMode(setPressurePotPin, INPUT);
  pinMode(setBPMPotPin, INPUT);
  pinMode(setIERatioPotPin, INPUT);
  pinMode(setTVPotPin, INPUT);

  //Switch input pin setup
  pinMode(setParameterPin, INPUT);
  pinMode(limitSwitchPin, INPUT);
  pinMode(alarmSwitchPin, INPUT);

  //Pressure sensor input pin setup
  pinMode(pressureSensorPin, INPUT);

  //Parameter change interrupt setup
  attachInterrupt(digitalPinToInterrupt(setParameterPin, parameterChangeButtonISR, FALLING));

  //LCD Setup
  lcd.begin(20, 4); //set number of columns and rows

  readPotentiometers(setThresholdPressurePotPin, setBPMPotPin, setIERatioPotPin, setTVPotPin, internalThresholdPressure, internalBPM, interalIERatio, internalTV);

  //LCD Display Startup Message for two seconds
  displayStartScreen(softwareVersion);
  

  cli(); //Turn off ointerrupts before reading paramChange
  while (paramChange == false) {

    sei();
    //LCD Display Internal Variables
    displayParameterScreen(internalTV, internalBPM, internalIERatio, internalThresholdPressure);




                           readPotentiometers(setThresholdPressurePotPin, setBPMPotPin, setIERatioPotPin, setTVPotPin, internalThresholdPressure, internalBPM, interalIERatio, internalTV);
                           cli();
  }
  paramChange = false;
  sei();

  //LCD Display Homing Message
  displayHomingScreen();
  

  //Motor Homing Sequence
  while (digitalRead(limitSwitchPin) == 0) {
    //Move motor at Vhome********
  }


  //Motor Zero Sequence
  while (digitalRead(limitSwitchPin) == 1) {
    //Move motor at Vzero********
  }

  //Hardcoded motor bag limit find sequence
  //Move motor x degrees inward********

  //Zero the encoder********
}

void loop() {

  //Update LCD*********
  cli(); //Prevent interrupts from occuring
  if (paramChange == true) {
    sei();
    readPotentiometers(setThresholdPressurePotPin, setBPMPotPin, setIERatioPotPin, setTVPotPin, tempThresholdPressure, tempBPM, tempIERatio, tempTV);

    //LCD display temp screen and variables
    displayParameterScreen(tempTV, tempBPM, tempIERatio, tempThresholdPressure);

  }
  else {
    sei();

    //LCD display internal variables and regular screen
    displayVentilationScreen(internalTV, internalBPM, internalIERatio, internalThresholdPressure, machineState, peakPressure, plateauPressure, peepPressure);
  }

  //Beginning of state machine code

  if (machineState == 2) { //BreathLoopStart---------------------------------------------------------------------------------

    cli();
    loopThresholdPressure = internalThresholdPressure;
    loopBPM = internalBPM;
    loopIERatio = internalIERatio;
    loopTV = internalTV;
    sei();

    float singleBreathTime = 60.0 / loopBPM;
    float inspirationTime = singleBreathTime / (1 + loopIERatio);
    float expirationTime = singleBreathTime - inspirationTime;

    if (digitalRead(modeSwitchPin) == ACMODE) {
      machineState = ACMode;
    }
    else {
      machineState = VCMode
    }
  }//----------------------------------------------------------------------------------------------------------------------
  //ACMode-----------------------------------------------------------------------------------------------------------------
  else if (machineState == 3) { //Check for ACMode
    if (acModeState == 0) { //ACStart
      breathTimer = 0;
      //Send motor to zero point******* (Consider watchdog timer for each state)
    }
    if (acModeState == 1) { //ACInhaleWait-------------------------------------------------------------------------------------

      readPressureSensor(pressureSensorPin, pressure);

      if (breathTimer > acThresholdTime * 1000) {
        acModeState = ACInhale;
        //SOUND LOW RR ALARM********
        breathtimer = 0;
        tempPeakPressure = 0;
      }
      else if (pressure < thresholdPressure) {
        acModeState = ACInhale;
        breathTimer = 0;
        tempPeakPressure = 0;
      }
    }//-----------------------------------------------------------------------------------------------------------------------
    else if (acModeState == 2) { //ACInhale-------------------------------------------------------------------------------------

      //Set motor velocity and position********

      readPressureSensor(pressureSensorPin, pressure);

      if (pressure > tempPeakPressure) { //Update the peak pressure
        tempPeakPressure = pressure;
      }

      if (breathTimer > inspirationTime * 1000) {
        breathTimer = 0;
        acModeState = ACPeak;
        peakPressure = tempPeakPressure;
      }
      else if (pressure > maxPressure) {
        //SOUND THE PRESSURE ALARM, scream inernally because we don't know what to do next
      }
    }//-----------------------------------------------------------------------------------------------------------------------
    else if (acModeState == 3) { //ACPeak-----------------------------------------------------------------------------------------

      //Hold motor in position********

      readPressureSensor(pressureSensorPin, pressure);

      if (breathTimer > holdTime * 1000) { //******** how and where is hold time defined, currently hard coded
        acModeState = ACExhale;
        plateaPressure = pressure;
        breathTimer = 0;
      }
      else if (pressure > maxPressure) {
        //SOUND THE PRESURE ALARM, scream inernally because we don't know what to do next
      }
    }//--------------------------------------------------------------------------------------------------------------------------
    else if (acModeState == 4) { //ACExhale---------------------------------------------------------------------------------------

      //Send motor to zero position********

      readPresureSensor(pressureSensorPin, pressure);

      if (breathTimer > expirationTime * 1000) {
        acModeState = ACReset;
        peepPressure = pressure;
      }
    }//--------------------------------------------------------------------------------------------------------------------------
    else if (acModeState == 5) { //ACReset-----------------------------------------------------------------------------------------

      readPressureSensor(pressureSensorPin, pressure);

      if (pressure > maxPeepPressure) {
        //SOUND THE ALARM******
      }
      else if (pressure < minPeepPressure) {
        //SOUND THE ALARM********
      }
      breathTimer = 0;
      acModeState = ACStart
                    machineState = BreathLoopStart
    }
  }//End ACMode
  else if (machineState == 4) { //VCMode-------------------------------------------------------------------------------------------
    if (vcModeState == 0) { //VCStart-----------------------------------------------------------------------------------------------
      breathTimer = 0;
      vcModeState = VCInhale;
      temppeakPressure = 0;
    }//---------------------------------------------------------------------------------------------------------------------------
    else if (vcModeState == 1) { //VCInhale---------------------------------------------------------------------------------------------

      //Set motor position and speed

      readPressureSensor(pressureSensorPin, pressure)

      if (pressure > tempPeakPressure) {
        tempPeakPressure = pressure;
      }

      if (breathTimer > inspirationTime * 1000) {
        vcModeState = VCPeak
                      breathTimer = 0;
        peakPressure = tempPeakPressure;
      }

      if (pressure > maxPressure) {
        //Scream loudly********
      }
    }//-------------------------------------------------------------------------------------------------------------------------
    else if (vcModeState == 2) { //VCPeak------------------------------------------------------------------------------------------

      //Hold motor in position********

      readPressureSensor(pressureSensorPin, pressure);

      if (breathTimer > holdTime * 1000) {
        vcModeState = 4;
        breathTimer = 0;
        plateauPressure = pressure;
      }

      if (pressure > maxPressure) {
        //Scream loudly*******************************
      }
    }//------------------------------------------------------------------------------------------------------------------------
    else if (vcModeState == 3) { //VCExhale-------------------------------------------------------------------------------------

      //Set motor vlocity and desired position

      readPressureSensor(pressureSensorPin, pressure);

      if (breathTimer > expirationTime) {
        vcModeState = VCReset;
        peepPressure = pressure;
      }

      if (pressure > maxPeepPressure) {
        //Yell*********
      }
      else if (pressure < minPeepPressure) {
        //Yell*********
      }
    }//-------------------------------------------------------------------------------------------------------------------------
    else if (vcModeState = 4) {

      machineState = BreathLoopStart;
      vcModeSate = VCStart;

    }
  }// End VCMode----------------------------------------------------------------------------------------------------------------------------







}

//FUNCTIONS

/*Function to read the pressure sensor
   Inputs:
    -The pressure sensor pin
   Outputs:
    -The pressure is output pass by reference
*/
void readPressureSensor(uint8_t pressureSensorPin, float &pressure) {

  uint16_t pressurePinADCReading = analogRead(pressureSensorPin);

  float pressureSensorVoltage = pressurePinADCReading;

  pressure = voltageToPressureConversion(pressureSensorVoltage);

  return;
}

/*Function to read the potentiometer inputs
   Inputs:
    -The Threshold pressure potentiometer pin
    -The BPM potentiometer pin
    -The IERatio potentiometer pin
    -The TV potentiometer pin
    -Pointers to the threshold pressure, BPM, IERatio and TV variables
*/
void readPotentiometers(uint8_t thresholdPressurePotPin, uint8_t bpmPotPin, uint8_t ieRatioPotPin, uint8_t tvPotPin, float &thresholdPressure, float &bpm, float &ieRatio, float &tv) {

  uint16_t setThresholdPressurePotPinADCReading = analogRead(thresholdPressurePotPin);
  uint16_t setBPMPotPinADCReading = analogRead(bpmPotPin);
  uint16_t setIERatioPotPinADCReading = analogRead(ieRatioPotPin);
  uint16_t setTVPotPinADCReading = analogRead(tvPotPin);

  float setThresholdPressurePotVoltage = setThresholdPressurePotPinADCReading * adcReadingToVoltsFactor;
  float setBPMPotVoltage = setBPMPotPinADCReading * adcReadingToVoltsFactor;
  float setIERatioPotVoltage = setIERatioPotPinADCReading * adcReadingToVoltsFactor;
  float setTVPotVoltage = setTVPotPinADCReading * adcReadingToVoltsFactor

                          thresholdPressure = voltsToSetThresholdPressureConversion(setThresholdPressurePotVoltage);
  bpm = voltsToBPMConversion(setBPMPotVoltage);
  ieRatio = voltsToIERatioConversion(setIERatioPotVoltage);
  tv = voltsToTVConversion(setTVPotVoltage);

  return;
}

/*Function to convert pressure sensor voltage to usable pressure value
   Inputs:
      -sensorVoltage: The voltage output by the sesnor, must be converted to volts from adc reading prior to use of function

   Outputs:
      -pressure in (psi? cmH2O)
*/
float voltageToPressureConversion(float sensorVoltage) {

  float pressure = (maxPressure - minPressure) / (maxPressureVoltage - minPressureVoltage) * sensorVoltage;

  pressure += minPressure;

  return pressure;
}

/*Function to convert the pressure potentiometer voltage to a desired set pressure
   Inputs:
      -potVoltage: The voltage output of the potentiometer, must be converted to volts from adc reading prior to use of function

   Outputs:
      -setPressure (in psi? cmH2O?)
*/
float voltageToSetThresholdPressureConversion(float potVoltage) {

  float setPressure = (maxSetPressure - minSetPressure) / (setPressurePotMaxVoltage - 0.0) * potVoltage;

  setPressure += minSetPressure;

  return setPressure;

}

/*Function to convert the breaths per minute potentiometer voltage to a desired breaths per minute
   Inputs:
      -potVoltage: The voltage output of the potentiometer, must be converted to volts from adc reading prior to use of function

   Outputs:
      -set breaths per mintue
*/
* /
float voltageToBPMConversion(float potVoltage) {

  float BPM = (maxBPM - minBPM) / (bpmPotMaxVoltage - 0.0) * potVoltage;

  BPM += minBPM;

  return BPM;
}

/*Function to convert inspiration - expiration ratio potentiometer voltage to a desired IE ratio
   Inputs:
      -potVoltage: The voltage output of the potentiometer, must be converted to volts from adc reading prior to use of function

   Outputs:
      -set IE ratio
*/
* /
float voltageToIERatioConversion(float potVoltage) {

  float IERatio = (maxIERatio - minIERatio) / (ieRatioPotMaxVoltage - 0.0) * potVoltage;

  IERatio += minIERatio;

  return IERatio;
}

/*Function to convert tidal volume percentage potentiometer voltage to a desired tidal volume percentage
   Inputs:
      -potVoltage: The voltage output of the potentiometer, must be converted to volts from adc reading prior to use of function

   Outputs:
      -set tidal volume percentage
*/
* /
float voltageToTVConversion(float potVoltage) {

  float TV = (maxTV - minTV) / (tvPotMaxVoltage - 0.0) * potVoltage;

  TV += minTV;

  return TV;
}

void parameterChangeButtonISR() {

  if (parameterChangeDebounceTimer > 0.1 * 1000) {
    if (paramChange == false) {
      paramChange = true;
    }
    else {
      paramChange = false;
      internalThresholdPressure = tempThresholdPressure;
      internalBPM = tempBPM;
      internalIERatio  = tempIERatio;
      internalTV = tempTV;
    }
    parameterChangeDebounceTimer = 0;
  }
}

//------------------------------------------------------------------------------------------------------------------------------
//LCD Support Functions...here be dragons...


/*Function to display the parameter change screen on the LCD
   Inputs:
      - temp values for TV, BPM, IERatio, ThresholdPressure
*/
void displayParameterScreen(float tempTV, float tempBPM, float tempIERatio, float tempThresholdPressure) {
  //Prep variable for output
  int lcdTV = roundAndCast(tempTV);
  int lcdBPM = roundAndCast(tempBPM);
  int lcdIERatio = roundAndCast(tempIERatio);
  int lcdThresholdPressure = roundAndCast(tempThresholdPressure);

  //Prep lines for output
  char parameterScreenL1[] = "PRESS SET TO CONFIRM";
  char parameterScreenL2[20];
  char parameterScreenL3[20];
  char parameterScreenL4[20];
  sprintf(parameterScreenL2, "TV=%3d%%   BPM=%2d/MIN", lcdTV, lcdBPM);
  sprintf(parameterScreenL3, "I:E=1:%1d", lcdIERatio);
  sprintf(parameterScreenL4, "AC_THRESHOLD=%2d MBAR", lcdThresholdPressure);

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

/*Function to display the ventilator (regular) screen on the LCD
   Inputs:
      - temp values for TV, BPM, IERatio, ThresholdPressure
      - Ventilaton mode
      - Pressure data for peak, plateau, peep
*/
void displayVentilationScreen(float intTV, float intBPM, float intIERatio, float intThresholdPressure, enum machineStates machineState, float peakPressure, float plateauPressure, float peepPressure){
  
  //Prep variable for output
  int lcdTV = roundAndCast(intTV);
  int lcdBPM = roundAndCast(intBPM);
  int lcdIERatio = roundAndCast(intIERatio);
  int lcdThresholdPressure = roundAndCast(intThresholdPressure);
  int lcdPeakPressure = roundAndCast(peakPressure);
  int lcdPlateauPressure = roundAndCast(plateauPressure);
  int lcdPeepPressure = roundAndCast(peepPressure);

  char ventilatorScreenL1[20];
  char ventilatorScreenL2[20];
  char ventilatorScreenL3[20];
  char ventilatorScreenL4[20];
  
  //Prep first line
  if (machineState == ACMode){
    sprintf(ventilatorScreenL1,"AC (ACT=%2d)  DATA:",lcdThresholdPressure);   
  }
  else if (machineState == VCMode){
    sprintf(ventilatorScreenL1,"VC        DATA:");
  }
  else{
    sprintf(ventilatorScreenL1,"N/A        DATA:");
  }
  //Prep remaining lines
  sprintf(ventilatorScreenL2, "TV=%3d%%   PEAK:%2d", lcdTV, lcdPeakPressure);
  sprintf(ventilatorScreenL3, "BPM=%2d/MIN    PLAT:%2d", lcdBPM, lcdPlateauPressure);
  sprintf(ventilatorScreenL4, "I:E=%1d        PEEP=%2d", lcdIERatio, lcdPeepPressure);

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

void displayStartScreen(const char softwareVersion[]){
  const char splashScreen[] = "CALGARY E-VENT";
  lcd.clear();
  lcd.print(splashScreen);
  lcd.setCursor(0, 1);
  lcd.print(softwareVersion);
  delay(2000);
}

void displayHomingScreen(){
  const char calibrationScreenL1[] = "Calibration in";
  const char calibrationScreenL2[] = "progress...";
  lcd.clear();
  lcd.print(calibrationScreenL1);
  lcd.setCursor(0, 1);
  lcd.print(calibrationScreenL2);
}

int roundAndCast(float x) {
  int new_var;
  new_var = (int) round(x);
}



