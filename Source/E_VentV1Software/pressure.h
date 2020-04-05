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


//Pressure Sensor Definitions---------------------------------------------------
const float minPressureSensorVoltage = 0.0;
const float maxPressureSensorVoltage = 5.0; // Assumed 0-5V sensor

// TODO: Double check these constants. I'm unclear on the units.
const float minPressure = 0.0; //Absolute psi,cmH20?
const float maxPressure = 3.0; //Absolute psi, cmH20?
//------------------------------------------------------------------------------

//Threshold Pressure Definitions------------------------------------------------
const float minThresholdPressure = 1.0;
const float maxThresholdPressure = 2.0;
//------------------------------------------------------------------------------

//Max & Min Pressures-----------------------------------------------------------
const float maxPeepPressure = 20.0; //cmH2O
const float minPeepPressure = 0.0; //cmH2O
//------------------------------------------------------------------------------

// Potentiometer information
const float setThresholdPressurePotMaxVoltage = 5.0;

// Pressure sensor pins
const int pressureSensorPin          = 10;
const int setThresholdPressurePotPin = 9;

// Functions

/* Function to read the pressure sensor on the pressureSensorPin.
 * Inputs: None.
 * Outputs:
 *  -The pressure is returned in a float
 *    + TODO: Units?
 */

float readPressureSensor(void);


/*Function to convert the pressure potentiometer voltage to a desired set pressure
  Inputs:
  -potVoltage: The voltage output of the potentiometer, must be converted to volts from adc reading prior to use of function

  Outputs:
  -setPressure (in psi? cmH2O?)
*/

float voltageToSetThresholdPressureConversion(const float potVoltage);


/*Function to convert pressure sensor voltage to usable pressure value
  Inputs:
  -sensorVoltage: The voltage output by the sesnor, must be converted to volts from adc reading prior to use of function

  Outputs:
  -pressure in (psi? cmH2O)
*/

float voltageToPressureConversion(const float sensorVoltage);

#endif // pressure_h
