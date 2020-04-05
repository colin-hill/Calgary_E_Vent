#include "pressure.h"

float readPressureSensor(){
    return voltageToPressureConversion(analogRead(pressureSensorPin));
}


float voltageToSetThresholdPressureConversion(const float potVoltage) {
    return (maxThresholdPressure - minThresholdPressure) / setThresholdPressurePotMaxVoltage * potVoltage + minThresholdPressure;
}


float voltageToPressureConversion(const float sensorVoltage) {
    return (maxPressure - minPressure) / (maxPressureSensorVoltage - minPressureSensorVoltage) * sensorVoltage + minPressure;
}
