#include "MachineStates.h"
#include "pressure.h"
#include "breathing.h"
#include "UserParameter.h"


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

    //Ventilation Primary Values -----------------------------------------------------------------------------
        //BPM
    state.breaths_per_minute = DEFAULT_BPM; //1/MIN
        //Tidal Volume
    state.tidal_volume = DEFAULT_TIDAL_VOLUME; //percentage (out of 100)

    //Pressure Values -----------------------------------------------------------------------------------------
    state.pressure = 0; //CM H2O; pressure sensing reading
        //PIP Pressure
    state.current_loop_peak_pressure = 0; //CM H2O; peak pressure of the current loop, running value
    state.peak_pressure = 0; //CM H2O; measured PIP value
        //PEEP Pressure
    state.peep_pressure = 0; //CM H2O; measured PEEP value
        //Plateau Pressure
    state.plateau_pressure = 0; //CM H2O; measured plateau pressure value
        //AC Mode Threshold Pressure
    state.ac_threshold_pressure = DEFAULT_THRESHOLD_PRESSURE; //CM H2O; value below PEEP required to trigger a breath

    //Timing Values--------------------------------------------------------------------------------------------
        //AC Mode Threshold Time
     state.ac_threshold_time = 0; //seconds;
        //Plateau Pause Time
     state.plateau_pause_time = DEFAULT_PLATEAU_PAUSE_TIME; //seconds;
        //Inspiration Time
     state.inspiration_time = DEFAULT_INSPIRATION_TIME;
        //Motor Return Time
     state.motor_return_time = 1.0; //seconds;

    //Mechanism Values -----------------------------------------------------------------------------------------
    state.controller_temperature = 0; //C?

    state.errors = 0;

    return state;
}

machineStates check_mode(void) {
    if (digitalRead(MODE_SWITCH_PIN) == ACMODE) {
        return ACMode;
    }
    else {
        return VCMode;
    }
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
