import serial
import time

arduino_port_0 = "COM9"
arduino_port_1 = "COM2"
arduino_port_2 = "COM3"
arduino_port_3 = "COM4"
arduino_port_4 = "COM5"
arduino_port_5 = "COM6"
arduino_port_6 = "COM7"

def main():
	
	ser_0 = serial.Serial(arduino_port_0,9600,timeout=1)
		#ser_1 = serial.Serial(arduino_port_1,9600,timeout=0)
		#ser_2 = serial.Serial(arduino_port_2,9600,timeout=0)
		#ser_3 = serial.Serial(arduino_port_3,9600,timeout=0)
		#ser_4 = serial.Serial(arduino_port_4,9600,timeout=0)
		#ser_5 = serial.Serial(arduino_port_5,9600,timeout=0)
		#ser_6 = serial.Serial(arduino_port_6,9600,timeout=0)
	#except SerialException:
		#print("One of the serial ports was not found")

	absolute_encoder_log = open("absolute_encoder_log.txt", "w")

	try:
		while True:

			arduino_0_response = ser_0.readline()
			#arduino_1_response = ser_1.readline()
			#arduino_2_response = ser_2.readline()
			#arduino_3_response = ser_3.readline()
			#arduino_4_response = ser_4.readline()
			#arduino_5_response = ser_5.readline()
			#arduino_6_response = ser_6.readline()

			arduino_0_response = arduino_0_response.decode("utf-8")
			arduino_0_response = arduino_0_response.replace('\r','')
			arduino_0_response = arduino_0_response.replace('\n','')

			#arduino_1_response = arduino_1_response.decode("utf-8")
			#arduino_1_response = arduino_1_response.replace('\r','')
			#arduino_1_response = arduino_1_response.replace('\n','')

			#arduino_2_response = arduino_2_response.decode("utf-8")
			#arduino_2_response = arduino_2_response.replace('\r','')
			#arduino_2_response = arduino_2_response.replace('\n','')

			#arduino_3_response = arduino_3_response.decode("utf-8")
			#arduino_3_response = arduino_3_response.replace('\r','')
			#arduino_3_response = arduino_3_response.replace('\n','')

			#arduino_4_response = arduino_4_response.decode("utf-8")
			#arduino_4_response = arduino_4_response.replace('\r','')
			#arduino_4_response = arduino_4_response.replace('\n','')

			#arduino_5_response = arduino_5_response.decode("utf-8")
			#arduino_5_response = arduino_5_response.replace('\r','')
			#arduino_5_response = arduino_5_response.replace('\n','')

			#arduino_6_response = arduino_6_response.decode("utf-8")
			#arduino_6_response = arduino_6_response.replace('\r','')
			#arduino_6_response = arduino_6_response.replace('\n','')

			written = False
			time_string = getTimeString()

			if arduino_0_response != 0:
				print(arduino_0_response)
				written = True
				absolute_encoder_log.write("Time: ")
				absolute_encoder_log.write(time_string)
				absolute_encoder_log.write(" ")
				absolute_encoder_log.write("Arduino 0: ")
				absolute_encoder_log.write(arduino_0_response)
				#absolute_encoder_log.write(",")

			#if arduino_1_response != 0:
				#written = True
				#absolute_encoder_log.write("Time: ")
				#absolute_encoder_log.write(time_string)
				#absolute_encoder_log.write("Arduino 1: ")
				#absolute_encoder_log.write(arduino_1_response)
				#absolute_encoder_log.write(",")

			#if arduino_2_response != 0:
				#written = True
				#absolute_encoder_log.write("Time: ")
				#absolute_encoder_log.write(time_string)
				#absolute_encoder_log.write("Arduino 2: ")
				#absolute_encoder_log.write(arduino_2_response)
				#absolute_encoder_log.write(",")

			#if arduino_3_response != 0:
				#written = True
				#absolute_encoder_log.write("Arduino 3: ")
				#absolute_encoder_log.write(arduino_3_response)
				#absolute_encoder_log.write(",")

			#if arduino_4_response != 0:
				#written = True
				#absolute_encoder_log.write("Time: ")
				#absolute_encoder_log.write(time_string)
				#absolute_encoder_log.write("Arduino 4: ")
				#absolute_encoder_log.write(arduino_4_response)
				#absolute_encoder_log.write(",")

			#if arduino_5_response != 0:
				#written = True
				#absolute_encoder_log.write("Time: ")
				#absolute_encoder_log.write(time_string)
				#absolute_encoder_log.write("Arduino 5: ")
				#absolute_encoder_log.write(arduino_5_response)
				#absolute_encoder_log.write(",")

			#if arduino_6_response != 0:
				#written = True
				#absolute_encoder_log.write("Time: ")
				#absolute_encoder_log.write(time_string)
				#absolute_encoder_log.write("Arduino 6: ")
				#absolute_encoder_log.write(arduino_6_response)
				#absolute_encoder_log.write(",")

			if written == True:
				absolute_encoder_log.write("\n")

		time.sleep(0.001)

	except KeyboardInterrupt:
			absolute_encoder_log.close()
			ser_0.close()
			#ser_1.close()
			#ser_2.close()
			#ser_3.close()
			#ser_4.close()
			#ser_5.close()
			#ser_6.close()

def getTimeString():
	current_time = time.localtime(time.time())
	day = current_time.tm_mday
	hour = current_time.tm_hour
	minute = current_time.tm_min
	second = current_time.tm_sec
	time_string = str(day) + "/" + str(hour) + ":" + str(minute) + ":" + str(second)
	return time_string

if __name__ == "__main__":
	main()

	






