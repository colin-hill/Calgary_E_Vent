#include "UserParameter.h"

UserParameter::UserParameter(float min, float max, float increment){
	
	this->minValue = min;
	this->maxValue = max;
	this->increment = increment;
	
	return;
}

void UserParameter::updateValue(){
	
	this->value = this->tmpValue;
	
}

void UserParameter::updateTmpValue(int32_t numEncoderSteps){
	
	float funcValue = this->tmpValue + this->increment*((float)numEncoderSteps);
	
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