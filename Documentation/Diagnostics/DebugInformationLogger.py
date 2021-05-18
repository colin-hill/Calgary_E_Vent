import serial
import time

def main():
	#Config
	arduino_port = 'COM5' #Check in device manager

	ser = serial.Serial(arduino_port,9600,timeout=1)
	print("Opening serial port {}".format(arduino_port))
	debug_log = open("debug_log.txt", "w")
	print("Opening debug log file")

	try:
		while True:
			char = ser.read()
			#time_string = getTimeString()
			print("Read serial port")
			debug_log.write(char.decode("utf-8"))
				#debug_log.write(time_string)
			print("Wrote to text file")

	except KeyboardInterrupt:
		debug_log.close()
		ser.close()
        
def getTimeString():
	current_time = time.localtime(time.time())
	day = current_time.tm_mday
	hour = current_time.tm_hour
	minute = current_time.tm_min
	second = current_time.tm_sec
	time_string = str(day) + "/" + str(hour) + ":" + str(minute) + ":" + str(second)
	return time_string

if __name__ == '__main__':
	main()	

