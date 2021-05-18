#include "AbsoluteEncoder.h"



void setAbsVoltageToTicks(AbsoluteEncoderStruct &absEncoder, const long int motorPosition){
	//This function should be called after zeroing the motor encoder and then moving in a set amount of ticks
	absEncoder.voltageToTicksSlope = motorPosition/(absEncoder.zeroPointVoltage - absEncoder.limitSwitchVoltage); //TODO need to handle division by zero
	absEncoder.voltageToTicksIntercept = -1*absEncoder.voltageToTicksSlope*absEncoder.zeroPointVoltage;
	return;
}


//Returns the voltage reading of the absolute encoder at it current point
float readAbsoluteEncoder(void){
	long adcReading = analogRead(ABS_ENCODER_PIN);
	#ifdef SERIAL_DEBUG
		Serial.print(F("Raw ADC reading: "));
		Serial.println(adcReading);
	#endif
	return convertADCToVolts(adcReading);
}

float convertADCToVolts(long adcReading){
	float voltageReading = adcReading/ADC_MAX_VALUE*ADC_MAX_VOLTAGE;
	#ifdef SERIAL_DEBUG
		Serial.print(F("voltage conversion: "));
		Serial.println(voltageReading);
	#endif


	return voltageReading;
}

float readAbsoluteEncoderTicks(AbsoluteEncoderStruct &absEncoder){
	//Convert from an encoder voltage to ticks
	float currentTicks = readAbsoluteEncoder()*absEncoder.voltageToTicksSlope + absEncoder.voltageToTicksIntercept;
	#ifdef SERIAL_DEBUG
		Serial.print(F("slope: "));
		Serial.println(absEncoder.voltageToTicksSlope);
		Serial.print(F("intercept: "));
		Serial.println(absEncoder.voltageToTicksIntercept);
		Serial.print(F("zeroPointVoltage: "));
		Serial.println(absEncoder.zeroPointVoltage);
		Serial.print(F("limitSwitchVoltage: "));
		Serial.println(absEncoder.limitSwitchVoltage);
	#endif
	return currentTicks;
}

void handle_abs_motor_recalibration(RoboClaw &controller_name, VentilatorState &state){

	//TODO: Check motor position, adjust encoder reading to zero if deviation is found
	float currentAbsTicks = readAbsoluteEncoderTicks(state.absEncoder);
	long int currentMotorTicks = controller_name.ReadEncM1(MOTOR_ADDRESS);

	#ifdef PYTHON_DEBUG
		//Serial.println(F("In Motor Recal Funtion..."));
		Serial.print(F("ABS: "));
		Serial.println(currentAbsTicks);
		Serial.print(F("Quad: "));
		Serial.println(currentMotorTicks);
	#endif

	if (abs(currentAbsTicks-currentMotorTicks) > ABS_TO_QUAD_TOLERANCE) {
		controller_name.SetEncM1(MOTOR_ADDRESS, currentAbsTicks);

		//#ifdef SERIAL_DEBUG
		//Serial.print(F("Position Tolerance outside allowable range of: "));
		//Serial.println(ABS_TO_QUAD_TOLERANCE);
		//#endif

	}

	return;
}

void handle_absolute_encoder_zeroing(RoboClaw &controller_name, VentilatorState &state){

	switch(state.zeroing_state) {
	case CommandMoveOff:
		//no action required
		break;
	case MoveOffLimitSwitch:
		//no action required
		break;
	case CommandHome:
		//no action required
		break;
	case MotorHomingWait:
		//no action required
		break;
	case CommandZero:
		//Measure the absolute encoder reading at the limit switch
		state.absEncoder.limitSwitchVoltage = readAbsoluteEncoder();
		break;		
	case MotorZeroingWait:
		//no action required
		break;
	case MotorZero:
		//Motor is now at zero point
		state.absEncoder.zeroPointVoltage = readAbsoluteEncoder();
		setAbsVoltageToTicks(state.absEncoder, QP_TO_ZEROPOINT);
		break;
	default:
		//Should not happen
		break;
	}

	return;

}

void handle_absolute_encoder_ACMode(RoboClaw &controller_name, VentilatorState &state){
	switch(state.ac_state) {
	case ACStart:
		//no action required	
		break;
	case ACInhaleWait:
		//no action required
		break;
	case ACInhaleCommand:
		#ifdef SERIAL_DEBUG
		Serial.print(F("ABS Encoder reading (start inhale): "));
		Serial.println(readAbsoluteEncoderTicks(state.absEncoder));
		#endif
		break;
	case ACInhale:
		//no action required
		break;
	case ACInhaleAbort:
		//no action required
		break;
	case ACPeak:
		//no action required
		break;
	case ACExhaleCommand:
		#ifdef SERIAL_DEBUG
		Serial.print(F("ABS Encoder reading (start exhale): "));
		Serial.println(readAbsoluteEncoderTicks(state.absEncoder));
		#endif
		//no action required
		break;
	case ACExhale:
		//no action required
		break;
	case ACReset:
		//no action required
		break;
	default:
		//Should not happen
		break;
	}
	return;

}

void handle_absolute_encoder_VCMode(RoboClaw &controller_name, VentilatorState &state){

	switch(state.vc_state) {
	case VCStart:
		break;
	case VCInhaleCommand:
		#ifdef SERIAL_DEBUG
		Serial.print(F("ABS Encoder reading (start inhale): "));
		Serial.println(readAbsoluteEncoderTicks(state.absEncoder));
		#endif
		break;
	case VCInhale:
		//no action required
		break;
	case VCInhaleAbort:
		break;
	case VCPeak:
		break;
	case VCExhaleCommand:
		#ifdef SERIAL_DEBUG
		Serial.print(F("ABS Encoder reading (start exhale): "));
		Serial.println(readAbsoluteEncoderTicks(state.absEncoder));
		#endif
		break;
	case VCExhale:
		//no action required
		break;
	case VCReset:
		break;
	default:
		//Should not happen
		break;
	}
	return;

}


void handle_absolute_encoder(RoboClaw &controller_name, VentilatorState &state){

	switch(state.machine_state) {
	case Startup:
		//should not happen
		break;
	case StartupHold:
		//Should not happen
		break;
	case MotorZeroing:
		handle_absolute_encoder_zeroing(controller_name, state);
		return;
	case BreathLoopStart:
		//Check abs encoder and reset motor quadrature encoder to zero if deviation is found
		handle_abs_motor_recalibration(controller_name, state);
		break;
	case ACMode:
		handle_absolute_encoder_ACMode(controller_name, state);
		return;
	case VCMode:
		handle_absolute_encoder_VCMode(controller_name, state);
		return;
	case FailureMode:
		//No action required
		break;
	default:
		//should not happen
		break;
	}

	return;
}
