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
                    MotorZeroing,
                    BreathLoopStart,
                    ACMode,
                    VCMode
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


// Structure containing the full state of the ventilator.
struct VentilatorState {
    machineStates machine_state;

    // TODO: See if we can do something better here.
    // I'm a little annoyed at having to have two of these.
    // Just need to be careful resetting them.
    vcModeStates vc_state;
    acModeStates ac_state;

    // Parameters
    unsigned long breath_time;

    float pressure;

    // TODO: is there a better name for this?
    float temp_peak_pressure;
    float peak_pressure;

    float peep_pressure;
    float plateau_pressure;

    float loop_threshold_pressure;

    uint16_t errors;
};

// Make sure there are the same number of characters as machine states!
// This is necessary for machineStateCodeAssignment
const char machineStateCodes[] = "SZBAV";


/* Converts a machine state to a single character to display for
   debug purposes.

   Input:
   - Machine State

   Output:
   - Character representing the machine state based on machineStateCodes.
 */
char machineStateCodeAssignment(machineStates machineState);

#endif
