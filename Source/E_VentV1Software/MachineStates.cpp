#include "MachineStates.h"

char machineStateCodeAssignment(machineStates machineState) {
    // Check if the machineStateCodes array is too small (additional state may have been added)
    if (machineState >= sizeof(machineStateCodes)) {
        return 'E';
    }

    return machineStateCodes[machineState];
}
