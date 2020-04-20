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

enum SelectedParameter{
                       e_ThresholdPressure,
                       e_BPM,
                       e_InspirationTime,
                       e_TidalVolume,
                       e_PlateauPauseTime,
                       e_HighPIPAlarm,
                       e_LowPIPAlarm,
                       e_HighPEEPAlarm,
                       e_LowPEEPAlarm,
                       e_LowPlateauPressureAlarm,
                       e_HighRespiratoryRateAlarm,
                       e_None
};

const uint8_t NUM_USER_PARAMETERS = 11;

const float THRESHOLD_PRESSURE_INCREMENT          = 1; //cmH2O
const float THRESHOLD_PRESSURE_DEFAULT            = 3;
const float BPM_INCREMENT                         = 1; //Breaths per Minute
const float BPM_DEFAULT                           = 20;
const float INSPIRATION_TIME_INCREMENT            = 0.1; //Seconds
const float INSPIRATION_TIME_DEFAULT              = 2;
const float TIDAL_VOLUME_INCREMENT                = 5; //% of bag
const float TIDAL_VOLUME_DEFAULT                  = 50;
const float PLATEAU_PAUSE_TIME_INCREMENT          = 0.05; //Seconds
const float PLATEAU_PAUSE_TIME_DEFAULT            = 0.3;
const float HIGH_PIP_ALARM_INCREMENT              = 1; //cmH2O
const float HIGH_PIP_ALARM_DEFAULT                = 40;
const float LOW_PIP_ALARM_INCREMENT               = 1; //cmH2O
const float LOW_PIP_ALARM_DEFAULT                 = 5.0;
const float HIGH_PEEP_ALARM_INCREMENT             = 1; //cmH2O
const float HIGH_PEEP_ALARM_DEFAULT               = 20;
const float LOW_PEEP_ALARM_INCREMENT              = 1; //cmH2O
const float LOW_PEEP_ALARM_DEFAULT                = 3;
const float LOW_PLATEAU_PRESSURE_ALARM_INCREMENT  = 1; //cmH2O
const float LOW_PLATEAU_PRESSURE_ALARM_DEFAULT    = 0;
const float HIGH_RESPIRATORY_RATE_ALARM_INCREMENT = 1; //Breaths per Minute
const float DEFAULT_HIGH_RESPIRATORY_RATE_ALARM   = 1;

class UserParameter {
public:
    float value;
    float tmpValue;
    uint8_t selectPin;
   
    UserParameter(const float minValue, const float maxValue, const float increment, const uint8_t pin, const float defaultValue, SelectedParameter name);
    void updateValue();
    void updateTmpValue(int32_t numEncoderSteps);
    void writeToNV();
    void readFromNV();

    static float currentInspirationTime;
    static float currentBPM;
    static float currentPlateauPauseTime;
		
private:
    float minValue;
    float maxValue;
    float increment;
    SelectedParameter name;
};

//UserParameter THRESHOLD_PRESSURE(MIN_THRESHOLD_PRESSURE,MAX_THRESHOLD_PRESSURE,thresholdPressureIncrement, thresholdPressureSelectPin);
//UserParameter BPM(MIN_BPM, MAX_BPM, bpmIncrement, bpmSelectPin);
//UserParameter IE_RATIO(MIN_IE_RATIO, MAX_IE_RATIO, ieRatioIncrement, ieRatioSelectPin);
//UserParameter TIDAL_VOLUME(MIN_TIDAL_VOLUME, MAX_TIDAL_VOLUME, tidalVolumeIncrement, tidalVolumeSelectPin);
#endif
