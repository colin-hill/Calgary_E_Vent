#include "Motor.h"


#include "alarms.h"
#include "conversions.h"


//Helper Functions

void setMotorZero(RoboClaw &controller_name) {
	controller_name.SetEncM1(MOTOR_ADDRESS, 0);
}

long int readPosition(RoboClaw &controller_name) {
  Serial.println("read position");
	long int pos = controller_name.ReadEncM1(MOTOR_ADDRESS);
  Serial.println(pos);
  return pos;
}


void commandStop(RoboClaw &controller_name) {
	controller_name.SpeedDistanceM1(MOTOR_ADDRESS, 0, 0, 1); //Stop motion
}


//Command Functions

void commandMotorHoming(RoboClaw &controller_name) {
	//command motor to move outwards
	controller_name.SpeedM1(MOTOR_ADDRESS, MOTOR_HOMING_SPEED);
}



VentilatorState commandMotorZero(RoboClaw &controller_name, VentilatorState state) {
	//Stop motion
	commandStop(controller_name);

	//Make sure the motor has come to a stop
	delay((HOMING_BUFFER*S_TO_MS));

	state.current_motor_position = readPosition(controller_name);
	state.future_motor_position = state.current_motor_position + QP_TO_ZEROPOINT;
	//Move to zeropoint
	controller_name.SpeedAccelDeccelPositionM1(MOTOR_ADDRESS, ACCEL, MOTOR_ZEROING_SPEED, DECCEL, QP_TO_ZEROPOINT, 1);

	return state;
}

VentilatorState commandInhale(RoboClaw &controller_name, VentilatorState state) { 
  #ifdef SERIAL_DEBUG
    Serial.println("Motor Inhale Command");
  #endif

	long int desired_position = (long int) state.motor_inhale_pulses;
	long int desired_speed = (long int) desired_position/state.inspiration_time;

	//Update expected location
	state.future_motor_position = desired_position;

	controller_name.SpeedAccelDeccelPositionM1(MOTOR_ADDRESS, ACCEL, desired_speed, DECCEL, desired_position, 1);

	return state;
}

VentilatorState commandExhale(RoboClaw &controller_name, VentilatorState state) {
  #ifdef SERIAL_DEBUG
    Serial.println("Motor Exhale Command");
  #endif

	long int desired_position = 0;
	long int desired_speed = (long int) state.motor_return_speed;

	//Update expected location
	state.future_motor_position = 0;

	//Change sign of speed to travel backwards
	controller_name.SpeedAccelDeccelPositionM1(MOTOR_ADDRESS, ACCEL, desired_speed, DECCEL, desired_position, 1);

	return state;
}

VentilatorState commandInhaleAbort(RoboClaw &controller_name, VentilatorState state) {
	
	long int desired_speed = (long int) state.motor_return_speed;
	//Stop Motion
	commandStop(controller_name);
	//Read Current Location
	long int desired_position = 0;
	state.future_motor_position = 0;

	//Command a return to zero
	controller_name.SpeedAccelDeccelPositionM1(MOTOR_ADDRESS, ACCEL, desired_speed, DECCEL, desired_position, 1);

	return state;
}

VentilatorState checkMotorStatus(RoboClaw &controller_name, VentilatorState state) {

  Serial.println("check motor status");

	//Check current position
	state.current_motor_position = readPosition(controller_name);

	state.errors |= check_motor_position(state.current_motor_position, state.future_motor_position);
 
	controller_name.ReadTemp(MOTOR_ADDRESS, state.controller_temperature);
	state.errors |= check_controller_temperature(state.controller_temperature);	

	
  Serial.println("exit check motor status");
	return state;
}



//State Machine Functions

VentilatorState handle_ACMode(RoboClaw &controller_name, VentilatorState state) {

  Serial.println("Motor ac mode handle");
	
	switch(state.ac_state) {
	case ACStart:
		//no action required
		break;
	case ACInhaleWait:
		//no action required
		break;
	case ACInhaleCommand:
		return commandInhale(controller_name, state);
	case ACInhale:
		//no action required
		break;
	case ACInhaleAbort:
		return commandInhaleAbort(controller_name, state);
	case ACPeak:
		return checkMotorStatus(controller_name, state);
	case ACExhaleCommand:
		return commandExhale(controller_name, state);
	case ACExhale:
		//no action required
	case ACReset:
		return checkMotorStatus(controller_name, state);
	default:
		//Should not happen
		break;
	}
	return state;
}

VentilatorState handle_VCMode(RoboClaw &controller_name, VentilatorState state) {

  Serial.println("Motor vc mode handle");

		switch(state.vc_state) {
	case VCStart:
		//no action required
		break;
	case VCInhaleCommand:
		return commandInhale(controller_name, state);
	case VCInhale:
		//no action required
		break;
	case VCInhaleAbort:
		return commandInhaleAbort(controller_name, state);
	case VCPeak:
		return checkMotorStatus(controller_name, state);
	case VCExhaleCommand:
		return commandExhale(controller_name, state);
	case VCExhale:
		//no action required
	case VCReset:
		return checkMotorStatus(controller_name, state);
	default:
		//Should not happen
		break;
	}

	return state;
}

VentilatorState handle_MotorZeroing(RoboClaw &controller_name, VentilatorState state) {
  Serial.println("Motor zeroing handle");

	switch(state.zeroing_state) {
	case CommandHome:
		commandMotorHoming(controller_name);
		break;
	case MotorHomingWait:
		//no action required
		break;
	case CommandZero:
		return commandMotorZero(controller_name, state);		
	case MotorZeroingWait:
		//no action required
		break;
	case MotorZero:
		state = checkMotorStatus(controller_name, state);
		setMotorZero(controller_name);
		return state;
	default:
		//Should not happen
		break;
	}

	return state;
}



VentilatorState handle_motor(RoboClaw &controller_name, VentilatorState state) {
  Serial.println("Motor handle");

	switch(state.machine_state) {
	case Startup:
		//should not happen
		break;
	case StartupHold:
		//Should not happen
		break;
	case MotorZeroing:
		return handle_MotorZeroing(controller_name, state);
	case BreathLoopStart:
		//No action required
		break;
	case ACMode:
		return handle_ACMode(controller_name, state);
	case VCMode:
		return handle_VCMode(controller_name, state);
	case FailureMode:
		commandStop(controller_name);
		break;
	default:
		//should not happen
		break;
	}

	return state;
}
