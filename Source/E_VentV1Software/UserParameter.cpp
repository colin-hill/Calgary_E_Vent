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
    else if(e_TidalVolume == this->name){
        this->currentTidalVolume = this->value;
    }	
}

void UserParameter::updateTmpValue(int32_t numEncoderSteps){

    if(e_BPM == this->name){
        this->maxValue = min(MAX_BPM,(60.0 / ((this->currentInspirationTime) + DEFAULT_PLATEAU_PAUSE_TIME + 2*INERTIA_BUFFER + CODE_LATENCY+ 0.25)));
    }
    else if(e_InspirationTime == this->name){
        
        float tmpNewMax = (60.0 / (this->currentBPM)) - ((DEFAULT_PLATEAU_PAUSE_TIME) + 2*INERTIA_BUFFER + (CODE_LATENCY) + 0.25);

        float tmpNewMin = (this->currentTidalVolume) / 135.0; //TODO: Get rid of this magic number, this is the motor speed conversion factor

        if(tmpNewMax < this->minValue){
            this->maxValue = 0.5; //TODO: get rid of this magic number, this is the default min inspiration time
        }
        else{
            this->maxValue = tmpNewMax;
        }

        //if(tmpNewMin > this->maxValue){
            //this->minValue = this->maxValue;
        //}
        //else{
            //this->minValue = tmpNewMin;
        //}
    }
    else if(e_TidalVolume){

        //float tmp = (this->currentInspirationTime)*135.0; //TODO: Fix magic number  

        //this->maxValue = tmp;
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
