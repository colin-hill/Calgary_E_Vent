#include "encoder.h"
#include "UserParameter.h"

const float maxThresholdPressure = 2.0;
const float minThresholdPressure = 1.0;
const float thresholdPressureIncrement = 0.01;
const float maxBPM = 40.0;
const float minBPM = 10.0;
const float bpmIncrement = 0.5;

const float maxIERatio = 4;
const float minIERatio = 1;
const float ieRatioIncrement = 0.5;

const float maxTidalVolume = 100.0;
const float minTidalVolume = 0.0;
const float tidalVolumeIncrement = 5.0;

const uint8_t paramSelectInterruptPin = 2;
const uint8_t paramEncoderPushButtonPin = 20;
const uint8_t paramEncoderPin1 = 18;
const uint8_t paramEncoderPin2 = 19;
const uint8_t thresholdPressureSelectPin = 4;
const uint8_t bpmSelectPin = 5;
const uint8_t ieRatioSelectPin = 6;
const uint8_t tidalVolumeSelectPin = 7;

volatile boolean paramSelect = false;
volatile boolean paramSet = false;

Encoder paramSelectEncoder(paramEncoderPin1,paramEncoderPin2);

enum SelectedParameter{
  None,
  ThresholdPressure,
  BPM,
  IERatio,
  TidalVolume
 };

SelectedParameter currentlySelectedParameter = None;

UserParameter thresholdPressure(minThresholdPressure,maxThresholdPressure,thresholdPressureIncrement);
UserParameter bpm(minBPM, maxBPM, bpmIncrement);
UserParameter ieRatio(minIERatio, maxIERatio, ieRatioIncrement);
UserParameter tidalVolume(minTidalVolume, maxTidalVolume, tidalVolumeIncrement);

void setup() {

  pinMode(paramSelectInterruptPin,INPUT);
  pinMode(paramEncoderPushButtonPin,INPUT);

  pinMode(thresholdPressureSelectPin,INPUT);
  pinMode(bpmSelectPin,INPUT);
  pinMode(ieRatioSelectPin,INPUT);
  pinMode(tidalVolumeSelectPin,INPUT);

  attachInterrupt(digitalPinToInterrupt(paramSelectInterruptPin),paramSelectISR,FALLING);
  attachInterrupt(digitalPinToInterrupt(paramEncoderPushButtonPin),paramSetISR,FALLING);
}

void loop() {

  cli();
  if(paramSet){
    
    if(currentlySelectedParameter == ThresholdPressure){
      thresholdPressure.updateValue();
    }
    else if(currentlySelectedParameter == BPM){
      bpm.updateValue();
    }
    else if(currentlySelectedParameter == IERatio){
      ieRatio.updateValue();
    }
    else if(currentlySelectedParameter == TidalVolume){
      tidalVolume.updateValue();
    }
    
    paramSet = false;
    paramSelect = false; //Avoid setting and still changing
  }
  sei();

  cli();
  if(paramSelect){
    sei();
    if(digitalRead(thresholdPressureSelectPin)){
      currentlySelectedParameter = ThresholdPressure;
    }
    else if(digitalRead(bpmSelectPin)){
      currentlySelectedParameter = BPM;
    }
    else if(digitalRead(ieRatioSelectPin)){
      currentlySelectedParameter = IERatio;
    }
    else if(digitalRead(tidalVolumeSelectPin)){
      currentlySelectedParameter = TidalVolume; 
    }
    else{
      currentlySelectedParameter = None;
    }
    cli();
    paramSelect = false;
    sei();
  }
  sei();
    //----------------------------------------------------

    if(currentlySelectedParameter != None){

      int32_t encoderTurns = paramSelectEncoder.read();
      paramSelectEncoder.write(0); //Reset the encoder count in between loops

      if(currentlySelectedParameter == ThresholdPressure){
        thresholdPressure.updateTmpValue(encoderTurns);
      }
      else if(currentlySelectedParameter == BPM){
        bpm.updateTmpValue(encoderTurns);
      }
      else if(currentlySelectedParameter == IERatio){
        ieRatio.updateTmpValue(encoderTurns);
      }
      else if(currentlySelectedParameter == TidalVolume){
        tidalVolume.updateTmpValue(encoderTurns);
      }     
   }
}

void paramSelectISR(){

  paramSelect = true;

}

void paramSetISR(){

  paramSet = true;

}
