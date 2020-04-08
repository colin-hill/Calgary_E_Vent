#include "updateUserParameters.h"

void setUpParameterSelectButtons(UserParameter &thresholdPressure, UserParameter &bpm, UserParameter &ieRatio, UserParameter &tidalVolume,
                                const uint8_t parameterEncoderPushButtonPin)
{
	
	pinMode(thresholdPressure.selectPin,INPUT);
	pinMode(bpm.selectPin,INPUT);
	pinMode(ieRatio.selectPin,INPUT);
	pinMode(tidalVolume.selectPin,INPUT);
	
	pinMode(parameterEncoderPushButtonPin,INPUT);

	attachInterrupt(digitalPinToInterrupt(parameterEncoderPushButtonPin),parameterSetISR,RISING);

}

void updateUserParameters(SelectedParameter &currentlySelectedParameter, volatile boolean &parameterSet,
            Encoder &parameterSelectEncoder, UserParameter &thresholdPressure, UserParameter &bpm,
            UserParameter &ieRatio, UserParameter &tidalVolume)
{
  setParameters(currentlySelectedParameter, parameterSet, thresholdPressure, bpm, ieRatio, tidalVolume);
  updateParameterValue(currentlySelectedParameter, parameterSelectEncoder,thresholdPressure, bpm, ieRatio, tidalVolume);
  updateSelectedParameter(currentlySelectedParameter, parameterSelectEncoder,
              thresholdPressure, bpm, ieRatio, tidalVolume);
}

void updateSelectedParameter(SelectedParameter &currentlySelectedParameter, 
							Encoder &parameterSelectEncoder, UserParameter &thresholdPressure, UserParameter &bpm,
							UserParameter &ieRatio, UserParameter &tidalVolume)
{							
  if(e_None == currentlySelectedParameter){
	  if(digitalRead(thresholdPressure.selectPin)){
		  currentlySelectedParameter = e_ThresholdPressure;
		  parameterSelectEncoder.write(0);
	  }
	  else if(digitalRead(bpm.selectPin)){
		  currentlySelectedParameter = e_BPM;
		  parameterSelectEncoder.write(0);
	  }
	  else if(digitalRead(ieRatio.selectPin)){
		  currentlySelectedParameter = e_IERatio;
		  parameterSelectEncoder.write(0);
	  }
	  else if(digitalRead(tidalVolume.selectPin)){
		  currentlySelectedParameter = e_TidalVolume;
		  parameterSelectEncoder.write(0);		
	  }
	  else{
		  currentlySelectedParameter = e_None;
	  }
  }
}

void updateParameterValue(SelectedParameter &currentlySelectedParameter, 
						Encoder &parameterSelectEncoder, UserParameter &thresholdPressure, UserParameter &bpm,
						UserParameter &ieRatio, UserParameter &tidalVolume)
{
	if(e_None != currentlySelectedParameter){
		int32_t encoderTurns = parameterSelectEncoder.read();
		parameterSelectEncoder.write(0); //Reset the encoder count in between loops

		if(e_ThresholdPressure == currentlySelectedParameter){
			thresholdPressure.updateTmpValue(encoderTurns);
		}
		else if(e_BPM == currentlySelectedParameter){
			bpm.updateTmpValue(encoderTurns);
		}
		else if(e_IERatio == currentlySelectedParameter){
			ieRatio.updateTmpValue(encoderTurns);
		}
		else if(e_TidalVolume == currentlySelectedParameter){
			tidalVolume.updateTmpValue(encoderTurns);
		}     
	}
}	

void setParameters(SelectedParameter &currentlySelectedParameter,
				volatile boolean &parameterSet, UserParameter &thresholdPressure, UserParameter &bpm, UserParameter &ieRatio,
				UserParameter &tidalVolume)
{
	cli();
	if(parameterSet){
		if(e_ThresholdPressure == currentlySelectedParameter){
			thresholdPressure.updateValue();
		}
		else if(e_BPM == currentlySelectedParameter){
			bpm.updateValue();
		}
		else if(e_IERatio == currentlySelectedParameter){
			ieRatio.updateValue();
		}
		else if(e_TidalVolume == currentlySelectedParameter){
			tidalVolume.updateValue();
		}
    
		parameterSet = false;
   currentlySelectedParameter = e_None;
	}
	sei();
}

/*
void parameterSelectISR(){
  
  PARAMETER_SELECT = true;
  
}

void parameterSetISR(){
  
  PARAMETER_SET = true;
  
}
*/
