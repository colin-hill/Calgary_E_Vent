#include "updateUserParameters.h"

void setUpParameterSelectButtons(UserParameter *userParameters, const uint8_t NUM_USER_PARAMETERS,
                                 const uint8_t parameterEncoderPushButtonPin)
{

    for(uint8_t i = 0; i < NUM_USER_PARAMETERS; i++){
        pinMode(userParameters[i].selectPin,INPUT_PULLUP); //Active LOW
    }
	
    pinMode(parameterEncoderPushButtonPin,INPUT_PULLUP); //Active LOW

    attachInterrupt(digitalPinToInterrupt(parameterEncoderPushButtonPin),parameterSetISR,FALLING); //Active LOW

}

void updateStateUserParameters(VentilatorState &state, SelectedParameter &currentlySelectedParameter, volatile boolean &parameterSet,
                                          Encoder &parameterSelectEncoder, UserParameter *userParameters, const uint8_t NUM_USER_PARAMETERS)
{
    setParameters(currentlySelectedParameter, parameterSet, userParameters);
    updateParameterTempValue(currentlySelectedParameter, parameterSelectEncoder,userParameters);
    updateSelectedParameter(currentlySelectedParameter, parameterSelectEncoder,
                            userParameters, NUM_USER_PARAMETERS);
    return;
}

void updateSelectedParameter(SelectedParameter &currentlySelectedParameter, 
                             Encoder &parameterSelectEncoder, UserParameter *userParameters, const uint8_t NUM_USER_PARAMETERS)
{							
    if(e_None == currentlySelectedParameter){
	  
        uint8_t selectedArrayIndex = 0;

        while(selectedArrayIndex < NUM_USER_PARAMETERS){
            if(SWITCH_PRESS == digitalRead(userParameters[selectedArrayIndex].selectPin)){
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
        case 10 :
            currentlySelectedParameter = e_HighRespiratoryRateAlarm;
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
                           float measuredPIP, float measuredPEEP, float currentPressure, const int LCD_MAX_STRING, UserParameter *userParameters)
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
                                     bpm, thresholdPressure, tidalVolume, inspirationTime, currentPressure, 
                                     measuredPIP, measuredPEEP, LCD_MAX_STRING);
    }
}

void displayAlarmParameters(SelectedParameter &currentlySelectedParameter, LiquidCrystal &displayName,UserParameter *userParameters)
{
    SelectedParameter currentParameter = e_HighPIPAlarm;
    float maxPIP = userParameters[(int)currentParameter].value;
    float tempMaxPIP = userParameters[(int)currentParameter].tmpValue;
  
    currentParameter = e_LowPIPAlarm;
    float minPIP = userParameters[(int)currentParameter].value;
    float tempMinPIP = userParameters[(int)currentParameter].tmpValue;
  
    currentParameter = e_HighPEEPAlarm;
    float maxPEEP = userParameters[(int)currentParameter].value;
    float tempMaxPEEP = userParameters[(int)currentParameter].tmpValue;
  
    currentParameter = e_LowPEEPAlarm;
    float minPEEP = userParameters[(int)currentParameter].value;
    float tempMinPEEP = userParameters[(int)currentParameter].tmpValue;
  
    currentParameter = e_LowPlateauPressureAlarm;
    float lowPlateauPressure = userParameters[(int)currentParameter].value;
    float tempLowPlateauPressure = userParameters[(int)currentParameter].tmpValue;

    currentParameter = e_HighRespiratoryRateAlarm;
    float highRespiratoryRateAlarm = userParameters[(int)currentParameter].value;
    float tempHighRespiratoryRateAlarm = userParameters[(int)currentParameter].tmpValue;

    switch(currentlySelectedParameter){
    case e_HighPIPAlarm:
        displayHighPressureChange(displayName, tempMaxPIP, LCD_MAX_STRING);
        break;

    case e_LowPIPAlarm:
        displayLowPressureChange(displayName, tempMinPIP, LCD_MAX_STRING);
        break;

    case e_HighPEEPAlarm:
        displayHighPEEPChange(displayName, tempMaxPEEP, LCD_MAX_STRING);
        break;

    case e_LowPEEPAlarm:
        displayLowPEEPChange(displayName, tempMinPEEP, LCD_MAX_STRING);
        break;

    case e_LowPlateauPressureAlarm:
        displayLowPlateauChange(displayName, tempLowPlateauPressure, LCD_MAX_STRING);
        break;

    case e_HighRespiratoryRateAlarm:
        //TODO: Display high respiatory rate alarm
        break;

    default:
        //TODO clean up
        //displayNoAlarm(displayName, maxPIP, minPIP, maxPEEP, minPEEP, lowPlateauPressure, LCD_MAX_STRING);
        break;
    }
}

void setStateParameters(VentilatorState &state, UserParameter *userParameters){	
    SelectedParameter selectedParameter = e_ThresholdPressure; //TODO: Find a better way to access the array
    state.ac_threshold_pressure = userParameters[(int)selectedParameter].value;
    selectedParameter = e_BPM;
    state.breaths_per_minute = userParameters[(int)selectedParameter].value;
    selectedParameter = e_PlateauPauseTime;
    state.plateau_pause_time = userParameters[(int)selectedParameter].value;
    selectedParameter = e_TidalVolume;
    state.tidal_volume = userParameters[(int)selectedParameter].value;
    selectedParameter = e_InspirationTime;
    state.inspiration_time = userParameters[(int)selectedParameter].value;
  
    return;
}

/*
  void parameterSelectISR(){
  
  PARAMETER_SELECT = true;
  
  }

  void parameterSetISR(){
  
  PARAMETER_SET = true;
  
  }
*/
