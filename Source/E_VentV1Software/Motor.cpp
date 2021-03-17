#include "Motor.h"


#include "alarms.h"
#include "conversions.h"


//Helper Functions

void setMotorZero(RoboClaw &controller_name) {
	controller_name.SetEncM1(MOTOR_ADDRESS, 0);
}

long int readPosition(RoboClaw &controller_name) {
	long int pos = controller_name.ReadEncM1(MOTOR_ADDRESS);

	#ifdef PYTHON_DEBUG
		Serial.print(F("Encoder Reading:"));
		Serial.println(pos);
	#endif

  	return pos;
}



void commandStop(RoboClaw &controller_name) {
	controller_name.SpeedDistanceM1(MOTOR_ADDRESS, 0, 0, 1); //Stop motion
}


//Command Functions

void commandMotorMoveOff(RoboClaw &controller_name) {
	controller_name.SetEncM1(MOTOR_ADDRESS, 0);
	controller_name.SpeedAccelDeccelPositionM1(MOTOR_ADDRESS, ACCEL, MOTOR_ZEROING_SPEED, DECCEL, QP_AT_FULL_STROKE, 1);
}

void commandMotorHoming(RoboClaw &controller_name) {
	commandStop(controller_name);
	//command motor to move outwards
	#ifdef SERIAL_DEBUG
		Serial.println(F("Motor home start"));
	#endif

	controller_name.SetEncM1(MOTOR_ADDRESS, 0);
	controller_name.SpeedAccelDeccelPositionM1(MOTOR_ADDRESS, ACCEL, MOTOR_ZEROING_SPEED, DECCEL, -1000, 1); //TODO could we set a hard limit?
}



void commandMotorZero(RoboClaw &controller_name, VentilatorState &state) {
	//Stop motion
	commandStop(controller_name);

	//Make sure the motor has come to a stop
	delay((HOMING_BUFFER*S_TO_MS));

	setAbsZeroPointVoltage(state.absEncoder);


	state.current_motor_position = readPosition(controller_name);
	#ifdef SERIAL_DEBUG
		Serial.println(F("Location at zero command"));
		Serial.println(state.current_motor_position);
	#endif

	controller_name.SetEncM1(MOTOR_ADDRESS, 0);

	state.future_motor_position = QP_TO_ZEROPOINT;
	//Move to zeropoint
	controller_name.SpeedAccelDeccelPositionM1(MOTOR_ADDRESS, ACCEL, MOTOR_ZEROING_SPEED, DECCEL, QP_TO_ZEROPOINT, 1);

	//delay(4000); //TODO fix this timing

	return;
}

void commandInhale(RoboClaw &controller_name, VentilatorState &state) { 
  #ifdef SERIAL_DEBUG
    Serial.println(F("Motor Inhale Command"));
  #endif

	long int desired_position = (long int) state.motor_inhale_pulses;
	long int desired_speed = (long int) desired_position/state.inspiration_time;

	//Update expected location
	state.future_motor_position = desired_position;

	controller_name.SpeedAccelDeccelPositionM1(MOTOR_ADDRESS, ACCEL, desired_speed, DECCEL, desired_position, 1);

	return;
}

void commandExhale(RoboClaw &controller_name, VentilatorState &state) {
  #ifdef SERIAL_DEBUG
    Serial.println(F("Motor Exhale Command"));
  #endif

	long int desired_position = 0;
	long int desired_speed = (long int) state.motor_return_speed;

	//Update expected location
	state.future_motor_position = 0;

	//Change sign of speed to travel backwards
	controller_name.SpeedAccelDeccelPositionM1(MOTOR_ADDRESS, ACCEL, desired_speed, DECCEL, desired_position, 1);

	return;
}

void commandInhaleAbort(RoboClaw &controller_name, VentilatorState &state) {
	
	long int desired_speed = (long int) state.motor_return_speed;
	//Stop Motion
	//commandStop(controller_name); Moved to the state machine
	//Read Current Location
	long int desired_position = 0;
	state.future_motor_position = 0;

	//Command a return to zero
	controller_name.SpeedAccelDeccelPositionM1(MOTOR_ADDRESS, ACCEL, desired_speed, DECCEL, desired_position, 1);

	return;
}

void checkMotorStatus(RoboClaw &controller_name, VentilatorState &state) {


	#ifdef SERIAL_DEBUG
  		Serial.println(F("check motor status"));
  	#endif


	//Check current position
	state.current_motor_position = readPosition(controller_name);

	state.errors |= check_motor_position(state.current_motor_position, state.future_motor_position);

	#ifdef SERIAL_DEBUG
  		Serial.println(F("exit check motor status"));
  	#endif

	return;
}



//State Machine Functions

void handle_ACMode(RoboClaw &controller_name, VentilatorState &state) {

	#ifdef SERIAL_DEBUG
 		Serial.println(F("Motor ac mode handle"));
  		Serial.print(F("AC State: "));
  		Serial.println(state.ac_state);
  	#endif
	
	switch(state.ac_state) {
	case ACStart:	
		//checkMotorStatus(controller_name, state);
		break;
	case ACInhaleWait:
		//no action required
		break;
	case ACInhaleCommand:
		commandInhale(controller_name, state);
		return;
	case ACInhale:
		//no action required
		break;
	case ACInhaleAbort:
		commandInhaleAbort(controller_name, state);
		return;
	case ACPeak:
		//checkMotorStatus(controller_name, state);
		return;
	case ACExhaleCommand:
		checkMotorStatus(controller_name, state);
		//state = checkMotorStatus(controller_name, state);
		commandExhale(controller_name, state);
		return;
	case ACExhale:
		//no action required
		break;
	case ACReset:
		checkMotorStatus(controller_name, state);
		break;
	default:
		//Should not happen
		break;
	}
	return;
}

void handle_VCMode(RoboClaw &controller_name, VentilatorState &state) {

	#ifdef SERIAL_DEBUG	
  		Serial.println(F("Motor vc mode handle"));
  	#endif

		switch(state.vc_state) {
	case VCStart:
		
		break;
	case VCInhaleCommand:
		commandInhale(controller_name, state);
		return;
	case VCInhale:
		//no action required
		break;
	case VCInhaleAbort:
		commandInhaleAbort(controller_name, state);
		return;
	case VCPeak:
		//checkMotorStatus(controller_name, state); //TODO is this taking too long?
		return;
	case VCExhaleCommand:
		checkMotorStatus(controller_name, state);
		commandExhale(controller_name, state);
		return;
	case VCExhale:
		//no action required
		break;
	case VCReset:
		checkMotorStatus(controller_name, state);
		break;
	default:
		//Should not happen
		break;
	}

	return;
}

void handle_MotorZeroing(RoboClaw &controller_name, VentilatorState &state) {

	#ifdef SERIAL_DEBUG
  		Serial.println(F("Motor zeroing handle"));
  	#endif

	switch(state.zeroing_state) {
	case CommandMoveOff:
		commandMotorMoveOff(controller_name);
		break;
	case MoveOffLimitSwitch:
		//no action required
		break;
	case CommandHome:
		commandMotorHoming(controller_name);
		break;
	case MotorHomingWait:
		//no action required
		break;
	case CommandZero:
		commandMotorZero(controller_name, state);
		return;		
	case MotorZeroingWait:
		//no action required
		break;
	case MotorZero:
		//Reset errors
		state.errors = 0;
		state.alarm_outputs = 0;
		//Check motor position
		checkMotorStatus(controller_name, state);

		setAbsVoltageToTicks(state.absEncoder, QP_TO_ZEROPOINT);

		setMotorZero(controller_name);
		//Set the abolute encoder to a new zeropoint
		setAbsZeroPointVoltage(state.absEncoder);

		return;
	default:
		//Should not happen
		break;
	}

	return;
}



void handle_motor(RoboClaw &controller_name, VentilatorState &state) {

	#ifdef SERIAL_DEBUG
		Serial.println(F("Motor handle"));
	#endif

	switch(state.machine_state) {
	case Startup:
		//should not happen
		break;
	case StartupHold:
		//Should not happen
		break;
	case MotorZeroing:
		handle_MotorZeroing(controller_name, state);
		return;
	case BreathLoopStart:
		//No action required
		break;
	case ACMode:
		handle_ACMode(controller_name, state);
		return;
	case VCMode:
		handle_VCMode(controller_name, state);
		return;
	case FailureMode:
		commandStop(controller_name);
		break;
	default:
		//should not happen
		break;
	}

	return;
}
