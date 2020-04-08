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

    float pressure;

    // TODO: is there a better name for this?
    float temp_peak_pressure;
    float peak_pressure;

    float controller_temperature;

    float peep_pressure;

    float plateau_pressure;

    float loop_threshold_pressure;

    float ac_threshold_time;

    float plateau_pause_time;

    uint16_t errors;
};

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
