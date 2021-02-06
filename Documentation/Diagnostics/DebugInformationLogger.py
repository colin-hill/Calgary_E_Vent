import serial

def main():
	#Config
	arduino_port = 'COM1' #Cheeck in device manager

	ser = serial.Serial(arduino_port,9600,timeout=1)
	print("Opening serial port {}".format(arduino_port))
	debug_log = open("debug_log.txt", "w")
	print("Opening debug log file")

	try:
		while True:
			char = ser.read()
			print("Read serial port")
			debug_log.write(char.decode("utf-8"))
			print("Wrote to text file")

	except KeyboardInterrupt:
		debug_log.close()
		ser.close()

if __name__ == '__main__':
	main()	

