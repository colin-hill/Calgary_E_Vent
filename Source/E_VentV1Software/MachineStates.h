/* State of the machine
 */

#ifndef MachineStates_h
#define MachineStates_h

// Make sure each state has a character in machineStatesCodes
// This is for debug information.
enum machineStates {
                    Startup,
                    MotorZeroing,
                    BreathLoopStart,
                    ACMode,
                    VCMode,
                    FailureMode
};

// Make sure there are the same number of characters as machine states!
// This is necessary for machineStateCodeAssignment
const char machineStateCodes[] = "SZBAVF";

/* Converts a machine state to a single character to display for
   debug purposes.

   Input:
   - Machine State

   Output:
   - Character representing the machine state based on machineStateCodes.
 */
char machineStateCodeAssignment(machineStates machineState);

#endif
