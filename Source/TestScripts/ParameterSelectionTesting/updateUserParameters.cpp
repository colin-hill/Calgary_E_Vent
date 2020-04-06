#include "updateUserParameters.h"

void setUpParameterSelectButtons(UserParameter &thresholdPressure, UserParameter &bpm, UserParameter &ieRatio, UserParameter &tidalVolume,
                                const uint8_t parameterSelectInterruptPin, const uint8_t parameterEncoderPushButtonPin)
{
	
	pinMode(thresholdPressure.selectPin,INPUT);
	pinMode(bpm.selectPin,INPUT);
	pinMode(ieRatio.selectPin,INPUT);
	pinMode(tidalVolume.selectPin,INPUT);
	
	pinMode(parameterSelectInterruptPin,INPUT);
	pinMode(parameterEncoderPushButtonPin,INPUT);
	
	attachInterrupt(digitalPinToInterrupt(parameterSelectInterruptPin),parameterSelectISR,FALLING);
	attachInterrupt(digitalPinToInterrupt(parameterEncoderPushButtonPin),parameterSetISR,FALLING);

}

void updateUserParameters(SelectedParameter &currentlySelectedParameter, volatile boolean &parameterSelect, volatile boolean &parameterSet,
            Encoder &parameterSelectEncoder, UserParameter &thresholdPressure, UserParameter &bpm,
            UserParameter &ieRatio, UserParameter &tidalVolume)
{
  setParameters(currentlySelectedParameter, parameterSelect, parameterSet, thresholdPressure, bpm, ieRatio, tidalVolume);
  updateParameterValue(currentlySelectedParameter, parameterSelectEncoder,thresholdPressure, bpm, ieRatio, tidalVolume);
  updateSelectedParameter(currentlySelectedParameter, parameterSelect, parameterSelectEncoder,
              thresholdPressure, bpm, ieRatio, tidalVolume);
}

void updateSelectedParameter(SelectedParameter &currentlySelectedParameter, 
							volatile boolean &parameterSelect, 
							Encoder &parameterSelectEncoder, UserParameter &thresholdPressure, UserParameter &bpm,
							UserParameter &ieRatio, UserParameter &tidalVolume)
{							
	cli();
	if(parameterSelect){
		sei();
		parameterSelectEncoder.write(0);
		if(digitalRead(thresholdPressure.selectPin)){
			currentlySelectedParameter = e_ThresholdPressure;
		}
		else if(digitalRead(bpm.selectPin)){
			currentlySelectedParameter = e_BPM;
		}
		else if(digitalRead(ieRatio.selectPin)){
			currentlySelectedParameter = e_IERatio;
		}
		else if(digitalRead(tidalVolume.selectPin)){
			currentlySelectedParameter = e_TidalVolume; 
		}
		else{
			currentlySelectedParameter = e_None;
		}
		cli();
		parameterSelect = false;
		sei();
	}
	sei();
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

void setParameters(SelectedParameter &currentlySelectedParameter, volatile boolean &parameterSelect,
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
		parameterSelect = false; //Avoid setting and still changing
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
