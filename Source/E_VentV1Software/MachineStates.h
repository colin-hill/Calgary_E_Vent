/* State of the machine
 */

#ifndef MachineStates_h
#define MachineStates_h

#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif


// Make sure each state has a character in machineStatesCodes
// This is for debug information.
enum machineStates {
                    Startup,
                    StartupHold,
                    MotorZeroing,
                    BreathLoopStart,
                    ACMode,
                    VCMode,
                    FailureMode
};


/* States for the AC Mode state machine.

   TODO: directions to state machine diagram.
 */
enum acModeStates {
                   ACStart,
                   ACInhaleWait,
                   ACInhaleCommand,
                   ACInhale,
                   ACInhaleAbort,
                   ACPeak,
                   ACExhaleCommand,
                   ACExhale,
                   ACReset
};


/* States for the VC Mode state machine.

   TODO: directions to state machine diagram.
 */
enum vcModeStates {
                   VCStart,
                   VCInhale,
                   VCInhaleCommand,
                   VCInhaleAbort,
                   VCPeak,
                   VCExhaleCommand,
                   VCExhale,
                   VCReset
};

/* States for the VC Mode state machine.

   TODO: directions to state machine diagram.
 */
enum zeroingStates {
                    CommandHome,
                    MotorHomingWait,
                    CommandZero,
                    MotorZeroingWait,
                    MotorZero
};


// Structure containing the full state of the ventilator.
struct VentilatorState {
    machineStates machine_state;

    // TODO: See if we can do something better here.
    // I'm a little annoyed at having to have two of these.
    // Just need to be careful resetting them.
    vcModeStates vc_state;
    acModeStates ac_state;
    zeroingStates zeroing_state;

    // Parameters
    // TODO: should we worry about overflow for these?
    unsigned long breath_time_start; // When timer was started (ms).
    unsigned long current_time;      // Current time (ms).

    //TODO: consider naming

    //Ventilation Primary Values -----------------------------------------------------------------------------
        //BPM
    float breaths_per_minute; //1/MIN
        //Tidal Volume
    float tidal_volume; //percentage (out of 100)

    //Pressure Values -----------------------------------------------------------------------------------------
    float pressure; //CM H2O; pressure sensing reading
        //PIP Pressure
    float current_loop_peak_pressure; //CM H2O; peak pressure of the current loop, running value
    float peak_pressure; //CM H2O; measured PIP value
        //PEEP Pressure
    float peep_pressure; //CM H2O; measured PEEP value
        //Plateau Pressure
    float plateau_pressure; //CM H2O; measured plateau pressure value
        //AC Mode Threshold Pressure
    float ac_threshold_pressure; //CM H2O; value below PEEP required to trigger a breath

    //Timing Values--------------------------------------------------------------------------------------------
        //AC Mode Threshold Time
     float ac_threshold_time; //seconds;
        //Plateau Pause Time
     float plateau_pause_time; //seconds;
        //Inspiration Time
     float inspiration_time;
          //Motor Return Time
     float motor_return_time; //seconds;

    //Mechanism Values -----------------------------------------------------------------------------------------
    float controller_temperature; //C?

    uint16_t errors;
};


// Bools for mode.
const bool ACMODE = true;
const bool VCMODE = false;

// Pin for reading machine mode switch.
const uint8_t MODE_SWITCH_PIN = 40;

// Make sure there are the same number of characters as machine states!
// This is necessary for machineStateCodeAssignment
const char machineStateCodes[] = "SHZBAVF";


/* Converts a machine state to a single character to display for
   debug purposes.

   Input:
   - Machine State

   Output:
   - Character representing the machine state based on machineStateCodes.
 */
char machineStateCodeAssignment(machineStates machineState);


/* Get initial state.
 */
VentilatorState get_init_state(void);


/* Get machine state from mode switch pin.
 */
machineStates check_mode(void);


/* Update state with current time + pressure readings.
 */
void update_state(VentilatorState &state);


/* Reset timer.
 */
void reset_timer(VentilatorState &state);


/* Get elapsed time in ms.
 */
unsigned long elapsed_time(const VentilatorState &state);

#endif
