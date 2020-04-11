/* Alarms for the ventilator.
 */

#ifndef alarms_h
#define alarms_h

#include <LiquidCrystal.h>
#include "MachineStates.h"
#include "updateUserParameters.h"
#include "UserParameter.h"
#include <assert.h>

#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

// Alarm Sound definitions
const float ALARM_SOUND_LENGTH = 0.5; //Seconds

// High PIP Alarm Definitions----------------
const float MAX_HIGH_PIP_ALARM = 40; //cmH2O
const float MIN_HIGH_PIP_ALARM = 10; //cmH2O
//------------------------------------------

// Low PIP Alarm Definitions-----------------
const float MAX_LOW_PIP_ALARM = 40; //cmH2O
const float MIN_LOW_PIP_ALARM = 0; //cmH2O
//------------------------------------------

// High PEEP Alarm Definitions---------------
const float MAX_HIGH_PEEP_ALARM = 30; //cmH2O
const float MIN_HIGH_PEEP_ALARM = 1; //cmH2O
//------------------------------------------

// Low PEEP Alarm Definitions----------------
const float MAX_LOW_PEEP_ALARM = 30; //cmH2O
const float MIN_LOW_PEEP_ALARM = 0; //cmH2O
//------------------------------------------

// Low Plateau Pressure Alarm Defintions-----
const float MAX_LOW_PLATEAU_PRESSURE_ALARM = 40; //cmH2O
const float MIN_LOW_PLATEAU_PRESSURE_ALARM = 1; //cmH2O
//------------------------------------------

// Alarm flags
const uint16_t HIGH_PRESSURE_ALARM   = 0x01 << 0;
const uint16_t LOW_PRESSURE_ALARM    = 0x01 << 1;
const uint16_t HIGH_PEEP_ALARM       = 0x01 << 2;
const uint16_t LOW_PEEP_ALARM        = 0x01 << 3;
const uint16_t DISCONNECT_ALARM      = 0x01 << 4;
const uint16_t HIGH_TEMP_ALARM       = 0x01 << 5;
const uint16_t APNEA_ALARM           = 0x01 << 6;
const uint16_t DEVICE_FAILURE_ALARM  = 0x01 << 7;
//const uint16_t PRESSURE_SENSOR_ALARM = 0x01 << 8;

// Functions for triggering alarms.

// TODO: THESE FUNCTIONS *MUST* BE TESTED

/* Function to check for high pressure.

   Input:
   - Takes in the current pressure value
   Output:
   - Returns error code for HIGH_PRESSURE_ALARM if there is high
     pressure, and 0 otherwise.
 */
uint16_t check_high_pressure(const float pressure);


/* Function to check for low pressure.

   Input:
   - Takes in the current pressure value
   Output:
   - Returns error code for HIGH_PRESSURE_ALARM if there is high
     pressure, and 0 otherwise.
 */
// TODO we NEVER check for low pressure anywhere!!!!
uint16_t check_low_pressure(const float pressure);

/* Function to check for both high / low pressure.

   Input:
   - Takes in the current pressure value
   Output:
   - Returns error code with HIGH_PRESSURE_ALARM set if there is high
     pressure, LOW_PRESSURE_ALARM set if there is low pressure, and
     0 otherwise.
 */
uint16_t check_pressure(const float pressure);


/* Function to check for high peep pressure.

   Input:
   - Takes in the current pressure value
   Output:
   - Returns error code for HIGH_PEEP_ALARM if there is high
     pressure, and 0 otherwise.
 */
uint16_t check_high_peep(const float pressure);


/* Function to check for low peep pressure.

   Input:
   - Takes in the current pressure value
   Output:
   - Returns error code for LOW_PEEP_ALARM if there is low
     pressure, and 0 otherwise.
 */
uint16_t check_low_peep(const float pressure);


/* Function to check for both high / low peep pressure.

   Input:
   - Takes in the current pressure value
   Output:
   - Returns error code with HIGH_PEEP_ALARM set if there is high
     pressure, LOW_PEEP_ALARM set if there is low pressure, and
     0 otherwise.
 */

uint16_t check_peep(const float pressure);


uint16_t check_controller_temperature(const uint16_t temperature);

uint16_t check_motor_position(const long int current_position, const long int expected_position);


/* Function to handle alarms

   Input:
   - Takes in error flags

   Postconditions:
   - TODO: currently does nothing, should raise alarms (LCD, beeping)
   - TODO: will reset the errors flag --- only in certain conditions.
   - TODO: can resent machine state!!!
 */
VentilatorState handle_alarms(volatile boolean &alarmReset, VentilatorState &state, LiquidCrystal &displayName, UserParameter *userParameters, SelectedParameter &currentlySelectedParameter);

/* TODO: Check alarms more frequently?

There's a few points where we check for specific alarms, e.g., in VCPeak we
check for high pressure. Is there a good reason not to perform *all* of the
checks? I think if any of these checks fails at any point an alarm should be
raised, right? Like in VCPeak or any of the other states you also shouldn't have
low pressure...?
*/

void reset_alarms(VentilatorState &state);

void setUpAlarmSwitch();

void alarmResetISR();
#endif // alarms_h
