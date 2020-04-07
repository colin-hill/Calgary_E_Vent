#include "UserParameter.h"
#include "updateUserParameters.h"
#include "Encoder.h"
#include "breathing.h"
#include "elapsedMillis.h"

UserParameter THRESHOLD_PRESSURE(MIN_THRESHOLD_PRESSURE,MAX_THRESHOLD_PRESSURE,THRESHOLD_PRESSURE_INCREMENT, THRESHOLD_PRESSURE_SELECT_PIN);
UserParameter BPM(MIN_BPM, MAX_BPM, BPM_INCREMENT, BPM_SELECT_PIN);
UserParameter IE_RATIO(MIN_IE_RATIO, MAX_IE_RATIO, IE_RATIO_INCREMENT, IE_RATIO_SELECT_PIN);
UserParameter TIDAL_VOLUME(MIN_TIDAL_VOLUME, MAX_TIDAL_VOLUME, TIDAL_VOLUME_INCREMENT, TIDAL_VOLUME_SELECT_PIN);

SelectedParameter CURRENTLY_SELECTED_PARAMETER = e_None;
SelectedParameter testPrintParameter = e_BPM;
Encoder PARAMETER_SELECT_ENCODER(PARAMETER_ENCODER_PIN_1, PARAMETER_ENCODER_PIN_2);

volatile boolean PARAMETER_SET = false;

elapsedMillis displayTimer;
elapsedMillis selectionDebounceTimer;
elapsedMillis setDebounceTimer;

void setup() {
  Serial.begin(9600);
  setUpParameterSelectButtons(THRESHOLD_PRESSURE, BPM, IE_RATIO, TIDAL_VOLUME, PARAMETER_ENCODER_PUSH_BUTTON_PIN);
  
}

void loop() {

  updateUserParameters(CURRENTLY_SELECTED_PARAMETER, PARAMETER_SET, PARAMETER_SELECT_ENCODER,
                       THRESHOLD_PRESSURE, BPM, IE_RATIO, TIDAL_VOLUME); 

  if(displayTimer > 1000){
    displayTimer = 0;
    Serial.print("Currently Selected Parameter: ");
    Serial.println(CURRENTLY_SELECTED_PARAMETER);
    Serial.print("Threshold Pressure Temp Value: ");
    Serial.println(THRESHOLD_PRESSURE.tmpValue);
    Serial.print("Threshold Pressure True Value: ");
    Serial.println(THRESHOLD_PRESSURE.value);
    Serial.print("BPM Temp Value: ");
    Serial.println(BPM.tmpValue);
    Serial.print("BPM True Value: ");
    Serial.println(BPM.value);
  }
}

void parameterSetISR(){

  if(setDebounceTimer > 250){
    setDebounceTimer = 0;  
    PARAMETER_SET = true;
  }
  
}
