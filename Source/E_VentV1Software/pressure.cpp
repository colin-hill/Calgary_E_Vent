#include "pressure.h"
//TODO: Replace the adc to voltage conversion factor in readPressureSensor
const float ADC_READING_TO_VOLTS_FACTOR_PRESSURE = 5.0/1024.0; //5 volts divided by 10 bits, will fix maginc number later
float readPressureSensor(){
    return voltageToPressureConversion(ADC_READING_TO_VOLTS_FACTOR_PRESSURE * (float)analogRead(PRESSURE_SENSOR_PIN));
}


float voltageToSetThresholdPressureConversion(const float potVoltage) {
    return (MAX_THRESHOLD_PRESSURE - MIN_THRESHOLD_PRESSURE) / SET_THRESHOLD_PRESSURE_POT_MAX_VOLTAGE * potVoltage + MIN_THRESHOLD_PRESSURE;
}


float voltageToPressureConversion(const float sensorVoltage) {
    return (MAX_PRESSURE - MIN_PRESSURE) / (MAX_PRESSURE_SENSOR_VOLTAGE - MIN_PRESSURE_SENSOR_VOLTAGE) * sensorVoltage + MIN_PRESSURE;
}
