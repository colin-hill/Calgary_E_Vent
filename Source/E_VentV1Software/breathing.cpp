#include "breathing.h"


float voltageToBPMConversion(const float potVoltage) {
    return (MAX_BPM - MIN_BPM) / BPM_POT_MAX_VOLTAGE * potVoltage + MIN_BPM;
}


float voltageToIERatioConversion(const float potVoltage){
    // TODO: Is this correct? Had MAX_IE_RATIO before.
    return (MAX_IE_RATIO - MIN_IE_RATIO) / IE_RATIO_POT_MAX_VOLTAGE * potVoltage + MIN_IE_RATIO;
}

float voltageToTVConversion(const float potVoltage){
    return (MAX_TV - MIN_TV) / TV_POT_MAX_VOLTAGE * potVoltage + MIN_TV;
}
