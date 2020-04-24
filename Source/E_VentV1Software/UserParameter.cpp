#include "UserParameter.h"

UserParameter::UserParameter(const float minValue, const float maxValue, const float increment, const uint8_t pin, const float defaultValue, SelectedParameter name){
	
    this->minValue = minValue;
    this->maxValue = maxValue;
    this->increment = increment;
    this->selectPin = pin;
    this->value = defaultValue;
    this->tmpValue = defaultValue;
    this->name = name;
	
    return;
}

void UserParameter::updateValue(){
	
    this->value = this->tmpValue;

    if(e_BPM == this->name){
        this->currentBPM = this->value;
    }
    else if(e_InspirationTime == this->name){
        this->currentInspirationTime = this->value;
    }
    //else if(e_PlateauPauseTime == this->name){
        //this->currentPlateauPauseTime = this->value;
    //}	
}

void UserParameter::updateTmpValue(int32_t numEncoderSteps){

    if(e_BPM == this->name){
        this->maxValue = min(MAX_BPM,(60.0 / ((this->currentInspirationTime) + this->currentPlateauPauseTime + 2*INERTIA_BUFFER + CODE_LATENCY+ 0.25)));
    }
    else if(e_InspirationTime == this->name){
        float tmp = (60.0 / (this->currentBPM)) - ((this->currentPlateauPauseTime) + 2*INERTIA_BUFFER + (CODE_LATENCY) + 0.25);

        if(tmp < this->minValue){
            this->maxValue = 0.5;
        }
        else{
            this->maxValue = tmp;
        }
    }
	
    float funcValue = this->tmpValue + (this->increment)*((float)numEncoderSteps);
	
    if(funcValue > this->maxValue){
		
        funcValue = this->maxValue;
		
    }
    else if(funcValue < this->minValue){
		
        funcValue = this->minValue;
		
    }
	
    this->tmpValue = funcValue;
	
}

void writeToNV(){
}

void readFromNV(){
}
