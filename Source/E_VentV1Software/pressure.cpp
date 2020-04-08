#include "pressure.h"

// TODO: Replace the adc to voltage conversion factor in readPressureSensor

void setUpPressureSensor(uint32_t pressureSensorBaudRate){
	
	Wire.begin(19200); //TODO: Fix so that this isn't a magic number
	
}

float readPressureSensor(){
    uint8_t numberOfBytesToRequest = 2;
    uint8_t msbStatusBitMask = 0b00111111;

    PRESSURE_SENSOR_I2C.requestFrom(PRESSURE_SENSOR_ADDRESS, numberOfBytesToRequest);

    delay(5);
    uint8_t MSB = PRESSURE_SENSOR_I2C.read();
    uint8_t LSB = PRESSURE_SENSOR_I2C.read();

    MSB &= msbStatusBitMask; //Remove first two bits as per documentation
    uint16_t output = (MSB<<8) | LSB;

    float pressure = (((output - MIN_DIGITAL_OUTPUT) * (MAX_SENSOR_PRESSURE - MIN_SENSOR_PRESSURE)) / (MAX_DIGITAL_OUTPUT - MIN_DIGITAL_OUTPUT)) - MIN_SENSOR_PRESSURE;
    pressure = pressure*PSI_TO_CMH2O;

    return pressure;
}
