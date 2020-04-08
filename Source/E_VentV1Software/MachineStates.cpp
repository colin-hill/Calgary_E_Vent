#include "MachineStates.h"
#include "pressure.h"
#include "breathing.h"


char machineStateCodeAssignment(machineStates machineState) {
    // Check if the machineStateCodes array is too small (additional state may have been added)
    if (machineState >= sizeof(machineStateCodes)) {
        return 'E';
    }

    return machineStateCodes[machineState];
}


VentilatorState get_init_state(void) {
    VentilatorState state;

    state.machine_state = BreathLoopStart;

    state.vc_state = VCStart;
    state.ac_state = ACStart;
    state.zeroing_state = CommandHome;

    state.breath_time_start = millis();
    state.current_time = state.breath_time_start;

    state.pressure = 0;

    state.temp_peak_pressure = 0;
    state.peak_pressure = 0;

    state.peep_pressure = 0;

    state.controller_temperature = 0;


    state.plateau_pressure = 0;

    state.loop_threshold_pressure = 0;

    state.ac_threshold_time = 0;

    state.plateau_pause_time = DEFAULT_PLATEAU_PAUSE_TIME;

    state.errors = 0;

    return state;
}

void update_state(VentilatorState &state) {
    state.pressure     = readPressureSensor();
    state.current_time = millis();
}

void reset_timer(VentilatorState &state) {
    state.breath_time_start = state.current_time;
}

unsigned long elapsed_time(const VentilatorState &state) {
    return state.current_time - state.breath_time_start;
}
