#ifndef USER_PARAMETER_H
#define USER_PARAMETER_H

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#elif defined(WIRING)
#include "Wiring.h"
#else
#include "WProgram.h"
#include "pins_arduino.h"
#endif

class UserParameter {
	public:
		float value;
		float tmpValue;
		UserParameter(float min, float max, float increment);
		void updateValue();
		void updateTmpValue(int32_t numEncoderSteps);
		void writeToNV();
		void readFromNV();
		
	private:
	
	float minValue;
	float maxValue;
	float increment;
};
#endif