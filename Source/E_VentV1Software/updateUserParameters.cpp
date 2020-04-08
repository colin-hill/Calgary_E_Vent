#include "updateUserParameters.h"

void setUpParameterSelectButtons(UserParameter *userParameters, const uint8_t NUM_USER_PARAMETERS,
                                const uint8_t parameterEncoderPushButtonPin)
{

  for(uint8_t i = 0; i < NUM_USER_PARAMETERS; i++){
    pinMode(userParameters[i].selectPin,INPUT);
  }
	
	pinMode(parameterEncoderPushButtonPin,INPUT);

	attachInterrupt(digitalPinToInterrupt(parameterEncoderPushButtonPin),parameterSetISR,RISING);

}

VentilatorState updateStateUserParameters(VentilatorState &state, SelectedParameter &currentlySelectedParameter, volatile boolean &parameterSet,
            Encoder &parameterSelectEncoder, UserParameter *userParameters, const uint8_t NUM_USER_PARAMETERS)
{
	setParameters(currentlySelectedParameter, parameterSet, userParameters);
	updateParameterTempValue(currentlySelectedParameter, parameterSelectEncoder,userParameters);
	updateSelectedParameter(currentlySelectedParameter, parameterSelectEncoder,
							userParameters, NUM_USER_PARAMETERS);
	return setStateParameters(state, userParameters);
}

void updateSelectedParameter(SelectedParameter &currentlySelectedParameter, 
							Encoder &parameterSelectEncoder, UserParameter *userParameters, const uint8_t NUM_USER_PARAMETERS)
{							
  if(e_None == currentlySelectedParameter){
	  
	  uint8_t selectedArrayIndex = 0;

    while(selectedArrayIndex < NUM_USER_PARAMETERS){
      if(digitalRead(userParameters[selectedArrayIndex].selectPin)){
         break;
      }
      else{
        selectedArrayIndex ++;
      }
    }
    
    switch(selectedArrayIndex){
      case 0 :
        currentlySelectedParameter = e_ThresholdPressure;
        break;
      case 1 :
        currentlySelectedParameter = e_BPM;
        break;
      case 2 :
        currentlySelectedParameter = e_InspirationTime;
        break;
      case 3 : 
        currentlySelectedParameter = e_TidalVolume;
        break;
      case 4 :
        currentlySelectedParameter = e_PlateauPauseTime;
        break;
      case 5 :
        currentlySelectedParameter = e_HighPIPAlarm;
        break;
      case 6:
        currentlySelectedParameter = e_LowPIPAlarm;
        break;
      case 7 :
        currentlySelectedParameter = e_HighPEEPAlarm;
        break;
      case 8 :
        currentlySelectedParameter = e_LowPEEPAlarm;
        break;
      case 9 :
        currentlySelectedParameter = e_LowPlateauPressureAlarm;
        break;
      default :
        currentlySelectedParameter = e_None;
        break;
    }
  }
}

void updateParameterTempValue(SelectedParameter &currentlySelectedParameter, 
						Encoder &parameterSelectEncoder, UserParameter *userParameters)
{
	if(e_None != currentlySelectedParameter){
		int32_t encoderTurns = parameterSelectEncoder.read();
		parameterSelectEncoder.write(0); //Reset the encoder count in between loops

		userParameters[(int)currentlySelectedParameter].updateTmpValue(encoderTurns);
	}
}	

void setParameters(SelectedParameter &currentlySelectedParameter,
				volatile boolean &parameterSet, UserParameter *userParameters)
{
	cli();
	if(parameterSet){
    if(e_None != currentlySelectedParameter){
      userParameters[(int)currentlySelectedParameter].updateValue();
    }
		parameterSet = false;
    currentlySelectedParameter = e_None;
	}
	sei();
}

void displayUserParameters(SelectedParameter &currentlySelectedParameter, LiquidCrystal &displayName, machineStates machineState, vcModeStates vcState, acModeStates acState, 
                          float measuredPIP, float measuredPlateau, const int LCD_MAX_STRING, UserParameter *userParameters)
{ 
  SelectedParameter currentParameter = e_BPM;
  float bpm = userParameters[(int)e_BPM].value;
  float tempBPM = userParameters[(int)e_BPM].tmpValue;
  
  currentParameter = e_ThresholdPressure;
  float thresholdPressure = userParameters[(int)e_ThresholdPressure].value;
  float tempThresholdPressure = userParameters[(int)e_ThresholdPressure].tmpValue;
  
  currentParameter = e_InspirationTime;
  float inspirationTime = userParameters[(int)e_InspirationTime].value;
  float tempInspirationTime = userParameters[(int)e_InspirationTime].tmpValue;
  
  currentParameter = e_TidalVolume;
  float tidalVolume = userParameters[(int)e_TidalVolume].value;
  float tempTidalVolume = userParameters[(int)e_TidalVolume].tmpValue;
  
  currentParameter = e_PlateauPauseTime;
  float plateauPauseTime = userParameters[(int)e_PlateauPauseTime].value;
  float tempPlateauPauseTime = userParameters[(int)e_PlateauPauseTime].tmpValue;

  switch(currentlySelectedParameter){
    case e_ThresholdPressure:
      displayThresholdPressureChange(displayName, tempThresholdPressure, LCD_MAX_STRING);
      break;

    case e_BPM:
      displayBPMChange(displayName, tempBPM, LCD_MAX_STRING);
      break;

    case e_InspirationTime:
      displayInspirationTimeChange(displayName, tempInspirationTime, LCD_MAX_STRING);
      break;

    case e_TidalVolume:
      displayTVChange(displayName, tempTidalVolume, LCD_MAX_STRING);
      break;

    case e_PlateauPauseTime:
      displayPauseTimeChange(displayName, tempPlateauPauseTime, LCD_MAX_STRING);
      break;

    default:
      displayVentilationParameters(displayName, machineState, vcState , acState, 
                                  bpm, thresholdPressure, tidalVolume, inspirationTime, plateauPauseTime, 
                                  measuredPIP, measuredPlateau, LCD_MAX_STRING);
  }
}

VentilatorState setStateParameters(VentilatorState &state, UserParameter *userParameters){
	
	SelectedParameter selectedParameter = e_ThresholdPressure; //TODO: Find a better way to access the array
	state.ac_threshold_pressure = userParameters[(int)selectedParameter].value;
	selectedParameter = e_BPM;
	state.breaths_per_minute = userParameters[(int)selectedParameter].value;
	selectedParameter = e_PlateauPauseTime;
	state.plateau_pause_time = userParameters[(int)selectedParameter].value;
	selectedParameter = e_TidalVolume;
	state.tidal_volume = userParameters[(int)selectedParameter].value;
	
	return state;
}

/*
void parameterSelectISR(){
  
  PARAMETER_SELECT = true;
  
}

void parameterSetISR(){
  
  PARAMETER_SET = true;
  
}
*/
