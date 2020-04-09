#include "UserParameter.h"

UserParameter::UserParameter(const float minValue, const float maxValue, const float increment, const uint8_t pin, const float defaultValue){
	
	this->minValue = minValue;
	this->maxValue = maxValue;
	this->increment = increment;
	this->selectPin = pin;
	this->value = defaultValue; //TODO: Change this to not be just the min
  this->tmpValue = defaultValue;
	
	return;
}

void UserParameter::updateValue(){
	
	this->value = this->tmpValue;
	
}

void UserParameter::updateTmpValue(int32_t numEncoderSteps){
	
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
