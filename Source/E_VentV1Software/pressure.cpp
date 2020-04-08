#include "pressure.h"
//TODO: Replace the adc to voltage conversion factor in readPressureSensor

void setUpPressureSensor(uint32_t pressureSensorBaudRate){
	
	PRESSURE_SENSOR_I2C.begin(pressureSensorBaudRate);
	
}
float readPressureSensor(uint8_t pressureSensorAddress, const float MAX_SENSOR_PRESSURE, const float MIN_SENSOR_PRESSURE, const uint16_t MAX_DIGITAL_OUTPUT, const uint16_t MIN_DIGITAL_OUTPUT){
	
	uint8_t numberOfBytesToRequest = 2;
	uint8_t msbStatusBitMask = 0b00111111;
	
	PRESSURE_SENSOR_I2C.requestFrom(pressureSensorAddresss, numberOfBytesToRequest);
	
	delay(5);
	uint8_t MSB = PRESSURE_SENSOR_I2C.read();
	uint8_t LSB = PRESSURE_SENSOR_I2C.read();
	
	MSB &= msbStatusBitMask; //Remove first two bits as per documentation
	uint16_t output = (MSB<<8) | LSB;
	
	pressure = (((output - MIN_DIGITAL_OUTPUT) * (MAX_SENSOR_PRESURE - MIN_SENSOR_PRESSURE)) / (MAX_DIGITAL_OUTPUT - MIN_DIGITAL_OUTPUT)) - MIN_SENSOR_PRESSURE;
	
	pressure = pressure*PSI_TO_CMH2O;
	
	return pressure;

}