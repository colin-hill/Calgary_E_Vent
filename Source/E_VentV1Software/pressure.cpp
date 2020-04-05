#include "pressure.h"

float readPressureSensor(){
    return voltageToPressureConversion(analogRead(PRESSURE_SENSOR_PIN));
}


float voltageToSetThresholdPressureConversion(const float potVoltage) {
    return (MAX_THRESHOLD_PRESSURE - MIN_THRESHOLD_PRESSURE) / SET_THRESHOLD_PRESSURE_POT_MAX_VOLTAGE * potVoltage + MIN_THRESHOLD_PRESSURE;
}


float voltageToPressureConversion(const float sensorVoltage) {
    return (MAX_PRESSURE - MIN_PRESSURE) / (MAX_PRESSURE_SENSOR_VOLTAGE - MIN_PRESSURE_SENSOR_VOLTAGE) * sensorVoltage + MIN_PRESSURE;
}
