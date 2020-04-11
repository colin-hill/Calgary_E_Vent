/* Functions and constants related to handling the pressure sensor for the
 * ventilator.
 *
 * There is functionality here for reading pressure values from the sensor, as
 * well as setting the threshold pressure.
 */

// TODO include guards
#ifndef pressure_h
#define pressure_h

#define NO_SENSOR_DEBUG

#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include "src/SBWire/SBWire.h"

//Pressure Sensor Definitions---------------------------------------------------
#define PRESSURE_SENSOR_I2C Wire
const float PSI_TO_CMH2O = 70.307;

const int PRESSURE_SENSOR_BAUD_RATE   = 9600;
const uint8_t PRESSURE_SENSOR_ADDRESS = 40; //Honeywell documentation

const uint16_t MAX_DIGITAL_OUTPUT = 14745; //Honeywell I2C comms documentation
const uint16_t MIN_DIGITAL_OUTPUT = 1638; //Honeywell I2C comms documentation

// TODO: Double check these constants. I'm unclear on the units.
const float MIN_SENSOR_PRESSURE = -1.0; //PSI Differential
const float MAX_SENSOR_PRESSURE = 1.0; //PSI Differential
//------------------------------------------------------------------------------



// Functions
/*
	Function to initialize the pressure sensor into the desired configuration

*/
void setUpPressureSensor(void);


/* Function to read the pressure sensor on the PRESSURE_SENSOR_PIN.
 * Inputs: None.
 * Outputs:
 *  -The pressure is returned in a float in cmH2O
 */
float readPressureSensor();

#endif // pressure_h
