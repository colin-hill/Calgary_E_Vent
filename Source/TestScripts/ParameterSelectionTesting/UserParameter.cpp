#include "UserParameter.h"

UserParameter::UserParameter(const float minVlaue, const float maxValue, const float increment, const uint8_t pin){
	
	this->minValue = minValue;
	this->maxValue = maxValue;
	this->increment = increment;
	this->selectPin = pin;
	
	return;
}

void UserParameter::updateValue(){
	
	this->value = this->tmpValue;
	
}

void UserParameter::updateTmpValue(int32_t numEncoderSteps){
	
	float funcValue = this->tmpValue + (this->increment)*((float)numEncoderSteps);
	
	if(funcValue > this->maxValue){
		
		funcValue = maxValue;
		
	}
	else if(funcValue < this->minValue){
		
		funcValue = minValue;
		
	}
	
	this->tmpValue = funcValue;
	
}

void writeToNV(){
}

void readFromNV(){
}
