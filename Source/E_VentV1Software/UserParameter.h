#ifndef USER_PARAMETERS_H
#define USER_PARAMETERS_H

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#elif defined(WIRING)
#include "Wiring.h"
#else
#include "WProgram.h"
#include "pins_arduino.h"
#endif

#include "breathing.h"

const uint8_t NUM_USER_PARAMETERS = 10;

const float THRESHOLD_PRESSURE_INCREMENT = 1; //cmH2O
const float BPM_INCREMENT = 1; //Breath per Minute
const float INSPIRATION_TIME_INCREMENT = 0.1; //Seconds
const float TIDAL_VOLUME_INCREMENT = 5; //% of bag
const float PLATEAU_PAUSE_TIME_INCREMENT = 0.05; //Seconds
const float HIGH_PIP_ALARM_INCREMENT = 1; //cmH2O
const float LOW_PIP_ALARM_INCREMENT = 1; //cmH2O
const float HIGH_PEEP_ALARM_INCREMENT = 1; //cmH2O
const float LOW_PEEP_ALARM_INCREMENT = 1; //cmH2O
const float LOW_PLATEAU_PRESSURE_ALARM_INCREMENT = 1; //cmH2O

class UserParameter {
	public:
		float value;
		float tmpValue;
		uint8_t selectPin;
   
		UserParameter(const float minValue, const float maxValue, const float increment, const uint8_t pin);
		void updateValue();
		void updateTmpValue(int32_t numEncoderSteps);
		void writeToNV();
		void readFromNV();
		
	private:
	
	float minValue;
	float maxValue;
	float increment;
};

//UserParameter THRESHOLD_PRESSURE(MIN_THRESHOLD_PRESSURE,MAX_THRESHOLD_PRESSURE,thresholdPressureIncrement, thresholdPressureSelectPin);
//UserParameter BPM(MIN_BPM, MAX_BPM, bpmIncrement, bpmSelectPin);
//UserParameter IE_RATIO(MIN_IE_RATIO, MAX_IE_RATIO, ieRatioIncrement, ieRatioSelectPin);
//UserParameter TIDAL_VOLUME(MIN_TIDAL_VOLUME, MAX_TIDAL_VOLUME, tidalVolumeIncrement, tidalVolumeSelectPin);
#endif
