#include "pressure.h"

// TODO: Replace the adc to voltage conversion factor in readPressureSensor

void setUpPressureSensor(void) {
    Wire.begin(PRESSURE_SENSOR_BAUD_RATE);
}


float readPressureSensor() {
    uint8_t numberOfBytesToRequest = 2;
    uint8_t msbStatusBitMask = 0b00111111;

    uint8_t numBytesRead = Wire.requestFrom(PRESSURE_SENSOR_ADDRESS, numberOfBytesToRequest);

    if (0 == numBytesRead) {
        // TODO: Add a device failure alarm here
    }

    delay(5);

    uint8_t MSB = Wire.read();
    uint8_t LSB = Wire.read();

    MSB &= msbStatusBitMask; //Remove first two bits as per documentation
    uint16_t output = (MSB<<8) | LSB;

    float pressure = (output - MIN_DIGITAL_OUTPUT)*(MAX_SENSOR_PRESSURE - MIN_SENSOR_PRESSURE)/(MAX_DIGITAL_OUTPUT - MIN_DIGITAL_OUTPUT) + MIN_SENSOR_PRESSURE;
    pressure = pressure*PSI_TO_CMH2O;

    return pressure;
}
