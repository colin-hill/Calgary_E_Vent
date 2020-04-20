/* Alarms for the ventilator.
 */

#ifndef alarms_h
#define alarms_h

#include <LiquidCrystal.h>
#include "MachineStates.h"
#include "updateUserParameters.h"
#include "UserParameter.h"
#include <assert.h>
#include "conversions.h"

#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

// Alarm Sound definitions
const float ALARM_SOUND_LENGTH = 0.5; //Seconds

const float ALARM_SILENCE_TIME = 30.0;

// High PIP Alarm Definitions----------------
const float MAX_HIGH_PIP_ALARM = 70; //cmH2O
const float MIN_HIGH_PIP_ALARM = 10; //cmH2O
//------------------------------------------

// Low PIP Alarm Definitions-----------------
const float MAX_LOW_PIP_ALARM = 40; //cmH2O
const float MIN_LOW_PIP_ALARM = 0; //cmH2O
//------------------------------------------

// High PEEP Alarm Definitions---------------
const float MAX_HIGH_PEEP_ALARM = 35; //cmH2O
const float MIN_HIGH_PEEP_ALARM = 5; //cmH2O
//------------------------------------------

// Low PEEP Alarm Definitions----------------
const float MAX_LOW_PEEP_ALARM = 15; //cmH2O
const float MIN_LOW_PEEP_ALARM = 1; //cmH2O
//------------------------------------------

// Low Plateau Pressure Alarm Defintions-----
const float MAX_LOW_PLATEAU_PRESSURE_ALARM = 40; //cmH2O
const float MIN_LOW_PLATEAU_PRESSURE_ALARM = 1; //cmH2O
//------------------------------------------

//High Respiratory Rate Alarm----------------
const float MAX_RESPIRATORY_RATE_ALARM = 10;
const float MIN_RESPIRATORY_RATE_ALARM = 0;
//-------------------------------------------

// Alarm flags
const uint16_t HIGH_PRESSURE_ALARM       = 0x01 << 0;
const uint16_t LOW_PRESSURE_ALARM        = 0x01 << 1;
const uint16_t HIGH_PEEP_ALARM           = 0x01 << 2;
const uint16_t LOW_PEEP_ALARM            = 0x01 << 3;
const uint16_t DISCONNECT_ALARM          = 0x01 << 4;
const uint16_t HIGH_RR_ALARM             = 0x01 << 5;
const uint16_t MISSED_TRIGGER_ALARM      = 0x01 << 6;
const uint16_t MECHANICAL_FAILURE_ALARM  = 0x01 << 7;
const uint16_t FULL_DEVICE_FAILURE       = 0x01 << 8;
//const uint16_t PRESSURE_SENSOR_ALARM = 0x01 << 8;
const uint16_t HIGH_RESPIRATORY_RATE      = 0x01 << 10;

// Functions for triggering alarms.

// TODO: THESE FUNCTIONS *MUST* BE TESTED

/** Function to check for high pressure.

   Input:
   - Takes in the current pressure value
   Output:
   - Returns error code for HIGH_PRESSURE_ALARM if there is high
     pressure, and 0 otherwise.
 */
uint16_t check_high_pressure(const float pressure, UserParameter *userParameters);


/** Function to check for low pressure.

   Input:
   - Takes in the current pressure value
   Output:
   - Returns error code for HIGH_PRESSURE_ALARM if there is high
     pressure, and 0 otherwise.
 */
// TODO we NEVER check for low pressure anywhere!!!!
uint16_t check_low_pressure(const float pressure, UserParameter *userParameters);

/** Function to check for both high / low pressure.

   Input:
   - Takes in the current pressure value
   Output:
   - Returns error code with HIGH_PRESSURE_ALARM set if there is high
     pressure, LOW_PRESSURE_ALARM set if there is low pressure, and
     0 otherwise.
 */
uint16_t check_pressure(const float pressure, UserParameter *userParameters);


/** Function to check for high peep pressure.

   Input:
   - Takes in the current pressure value
   Output:
   - Returns error code for HIGH_PEEP_ALARM if there is high
     pressure, and 0 otherwise.
 */
uint16_t check_high_peep(const float pressure, UserParameter *userParameters);


/** Function to check for low peep pressure.

   Input:
   - Takes in the current pressure value
   Output:
   - Returns error code for LOW_PEEP_ALARM if there is low
     pressure, and 0 otherwise.
 */
uint16_t check_low_peep(const float pressure, UserParameter *userParameters);


/** Function to check for both high / low peep pressure.

   Input:
   - Takes in the current pressure value
   Output:
   - Returns error code with HIGH_PEEP_ALARM set if there is high
     pressure, LOW_PEEP_ALARM set if there is low pressure, and
     0 otherwise.
 */

uint16_t check_peep(const float pressure, UserParameter *userParameters);


//uint16_t check_controller_temperature(const uint16_t temperature);

uint16_t check_motor_position(const long int current_position, const long int expected_position);

uint16_t check_respiratory_rate(VentilatorState &state, UserParameter *userParameters);



/** Function to handle alarms

   Input:
   - Takes in error flags

   Postconditions:
   - TODO: currently does nothing, should raise alarms (LCD, beeping)
   - TODO: will reset the errors flag --- only in certain conditions.
   - TODO: can resent machine state!!!
 */
//VentilatorState handle_alarms(volatile boolean &alarmReset, VentilatorState &state, LiquidCrystal &displayName, UserParameter *userParameters, SelectedParameter &currentlySelectedParameter);

/* TODO: Check alarms more frequently?

There's a few points where we check for specific alarms, e.g., in VCPeak we
check for high pressure. Is there a good reason not to perform *all* of the
checks? I think if any of these checks fails at any point an alarm should be
raised, right? Like in VCPeak or any of the other states you also shouldn't have
low pressure...?
*/

void alarm_debounce_reset(VentilatorState &state);

void loop_alarm_manager(elapsedMillis &alarmSilenceTimer, volatile boolean &alarmReset, LiquidCrystal &alarmDisplay, LiquidCrystal &parameterDisplay, VentilatorState &state, UserParameter *userParameters, SelectedParameter &currentlySelectedParameter);

void control_alarm_output(elapsedMillis &alarmSilenceTimer, volatile boolean &alarmReset, VentilatorState &state);

void control_alarm_displays(LiquidCrystal &alarmDisplay, LiquidCrystal &parameterDisplay, VentilatorState &state, UserParameter *userParameters, SelectedParameter &currentlySelectedParameter);

void setUpAlarmPins();

void alarmResetISR();
#endif // alarms_h
