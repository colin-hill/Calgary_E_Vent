#include "UserParameter.h"
#include "updateUserParameters.h"
#include "Encoder.h"
#include "breathing.h"

UserParameter THRESHOLD_PRESSURE(MIN_THRESHOLD_PRESSURE,MAX_THRESHOLD_PRESSURE,thresholdPressureIncrement, thresholdPressureSelectPin);
UserParameter BPM(MIN_BPM, MAX_BPM, bpmIncrement, bpmSelectPin);
UserParameter IE_RATIO(MIN_IE_RATIO, MAX_IE_RATIO, ieRatioIncrement, ieRatioSelectPin);
UserParameter TIDAL_VOLUME(MIN_TIDAL_VOLUME, MAX_TIDAL_VOLUME, tidalVolumeIncrement, tidalVolumeSelectPin);

SelectedParameter CURRENTLY_SELECTED_PARAMETER = e_None;
Encoder PARAMETER_SELECT_ENCODER(parameterEncoderPin1, parameterEncoderPin2);

volatile boolean PARAMETER_SELECT = false;
volatile boolean PARAMETER_SET = false;


void setup() {

  setUpParameterSelectButtons(THRESHOLD_PRESSURE, BPM, IE_RATIO, TIDAL_VOLUME, parameterSelectInterruptPin, parameterEncoderPushButtonPin);
  
}

void loop() {

  updateUserParameters(CURRENTLY_SELECTED_PARAMETER, PARAMETER_SELECT, PARAMETER_SET, PARAMETER_SELECT_ENCODER,
                       THRESHOLD_PRESSURE, BPM, IE_RATIO, TIDAL_VOLUME); 

}

void parameterSelectISR(){
  
  PARAMETER_SELECT = true;
  
}

void parameterSetISR(){
  
  PARAMETER_SET = true;
  
}
