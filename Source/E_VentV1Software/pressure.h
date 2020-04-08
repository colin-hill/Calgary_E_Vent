/* Functions and constants related to handling the pressure sensor for the
 * ventilator.
 *
 * There is functionality here for reading pressure values from the sensor, as
 * well as setting the threshold pressure.
 */

// TODO include guards
#ifndef pressure_h
#define pressure_h

#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include "Wire.h"

//Pressure Sensor Definitions---------------------------------------------------
#define PRESSURE_SENSOR_I2C Wire
const float PSI_TO_CMH2O = 70.307;
const uint32_t PRESSURE_SENSOR_BAUD_RATE = 9600;
const uint8_t PRESSURE_SENSOR_ADDRESS = 40; //Honeywell documentation

const uint32_t MAX_DIGITAL_OUTPUT = 14745; //Honeywell I2C comms documentation
const uint32_t MIN_DIGITAL_OUTPUT = 1638; //Honeywell I2C comms documentation

// TODO: Double check these constants. I'm unclear on the units.
const float MIN_SENSOR_PRESSURE = -1.0; //PSI Differential
const float MAX_SENSOR_PRESSURE = 1.0; //PSI Differential
//------------------------------------------------------------------------------

const float MAX_PRESSURE = 40.0; //cmH2O
const float MIN_PRESSURE = 0.0; //cmH2O

//Threshold Pressure Definitions------------------------------------------------
const float MIN_THRESHOLD_PRESSURE = 1.0;
const float MAX_THRESHOLD_PRESSURE = 2.0;
//------------------------------------------------------------------------------

//Max & Min Pressures-----------------------------------------------------------
const float MAX_PEEP_PRESSURE = 20.0; //cmH2O
const float MIN_PEEP_PRESSURE = 0.0; //cmH2O
//------------------------------------------------------------------------------


// Functions
/*
	Function to initialize the pressure sensor into the desired configuration
	
*/
void setUpPressureSensor(const uint32_t PRESSURE_SENSOR_BAUD_RATE);

/* Function to read the pressure sensor on the PRESSURE_SENSOR_PIN.
 * Inputs: None.
 * Outputs:
 *  -The pressure is returned in a float in cmH2O
 */

float readPressureSensor();


#endif // pressure_h
