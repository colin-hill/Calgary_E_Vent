#include "MachineStates.h"
#include "pressure.h"
#include "breathing.h"
#include "UserParameter.h"
#include "PinAssignments.h"
#include "Motor.h"
#include "conversions.h"


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
    state.alarm_silence_start_time = state.breath_time_start;

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
        //Nominal expiration time
     state.expiration_time = SECONDS_PER_MINUTE/state.breaths_per_minute - state.inspiration_time - state.plateau_pause_time;

     state.calculated_respiratory_rate = DEFAULT_BPM;

     state.breath_counter = 0;
     

    //Mechanism Values -----------------------------------------------------------------------------------------
        //Controller temperature
    state.controller_temperature = 0; //C?
        //Distance of motor travel during inhale
    state.motor_inhale_pulses = 0.01*DEFAULT_TIDAL_VOLUME*QP_AT_FULL_STROKE;
        //Speed of motor during inhale
    state.motor_inhale_speed = state.motor_inhale_pulses/state.inspiration_time; //QPPS
       //Motor Return Time
    state.motor_return_time = state.expiration_time*MOTOR_RETURN_FACTOR; //seconds;

    state.motor_return_speed = state.motor_inhale_pulses/state.motor_return_time; //QPPS

    state.future_motor_position = 0;
    state.current_motor_position = 0;



    //Errors
    state.errors = 0;

    state.this_breath_errors = 0;
    state.last_breath_errors = 0;
    state.last_loop_errors = 0;
    state.alarm_outputs = 0;
    state.silenced_alarms = 0;

    state.mechanical_failure_count = 0;

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

void update_motor_settings(VentilatorState &state) {

    //TODO: need to deal with invalid user input combinations

    state.motor_inhale_pulses = 0.01*state.tidal_volume*QP_AT_FULL_STROKE;
    state.motor_inhale_speed = state.motor_inhale_pulses/state.inspiration_time;
    state.expiration_time = SECONDS_PER_MINUTE/state.breaths_per_minute - state.inspiration_time; //This can go negative
    state.motor_return_time = state.expiration_time*MOTOR_RETURN_FACTOR;
    state.motor_return_speed = state.motor_inhale_pulses/state.motor_return_time;

    state.ac_threshold_time = state.expiration_time - state.motor_return_time;

}

unsigned long elapsed_time(const VentilatorState &state) {
    return state.current_time - state.breath_time_start;
}

unsigned long elapsed_alarm_time(const VentilatorState &state) {
    return state.current_time - state.alarm_silence_start_time;
}

void reset_alarm_timer(VentilatorState &state) {
    state.alarm_silence_start_time = state.current_time;
}

unsigned long elapsed_respiratory_rate_time(const VentilatorState &state){

    return state.current_time - state.respiratory_rate_time;
}

void reset_respiratory_rate_timer(VentilatorState &state){

    state.respiratory_rate_time = state.current_time;
}

void calculate_respiratory_rate(VentilatorState &state){

    unsigned long elapsed_time = elapsed_respiratory_rate_time(state);

    state.calculated_respiratory_rate = (state.breath_counter / (elapsed_time / S_TO_MS)) * 60.0;

    state.breath_counter = 0;

    reset_respiratory_rate_timer(state);

}