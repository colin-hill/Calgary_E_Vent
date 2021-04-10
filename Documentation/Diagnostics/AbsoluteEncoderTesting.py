import serial
import re
import time

def main():
	#Config
	arduino_port = 'COM3' #Cheeck in device manager
	column_keys = ["Breath - Time"]
	time_string = getTimeString()
	row = [time_string]
	new_row = re.compile(r"\*\*\*LOOPSTART\*\*\*")
	key_value_pair = re.compile(".*:.*")
	breath_count = 0

	ser = serial.Serial(arduino_port,9600,timeout=1)
	print("Opening serial port {}".format(arduino_port))
	debug_log = open("debug_log.txt", "w")
	print("Opening debug log file")

	try:
		while True:
			line = ser.readline()
			print("Read serial port")
			line = line.decode("utf-8")
			line = line.replace('\r','')
			line = line.replace('\n','')
			print("Current serial read:{}".format(line))
			print(column_keys)
			if re.match(new_row,line):
				breath_count = breath_count+1
				#If a new breath is starting write the previous breath to file
				for value_index in range(len(row)-1):
					debug_log.write("{:<25}".format(str(row[value_index]) + ","))
				debug_log.write("{:<25}".format(str(row[-1])))
				debug_log.write("\n")
				time_string = getTimeString()
				resetRow(row)
				print(row)
				row[0] = str(breath_count) + " " + time_string
			
			elif re.match(key_value_pair,line):
				print(line)
				split_key_value = line.split(":")
				print("This is the split: {}".format(split_key_value))
				new_key = str(split_key_value[0])
				print("This is the new key: {}".format(new_key))
				new_value = str(split_key_value[1])
				print("This is the new value: {}".format(new_value))

				key_index = getKeyIndex(column_keys,row,new_key)
				print(row)
				print(key_index)
				row[key_index] = new_value


	except KeyboardInterrupt:
		header_string = ""
		for keys in column_keys:
			header_string = header_string + "{:<25}".format(str(keys))
		line_appender(debug_log,header_string)
		debug_log.close()
		ser.close()

def getKeyIndex(column_keys,row, new_key):
	
	if new_key in column_keys:
		index = column_keys.index(new_key)
		return index
	else:
		column_keys.append(new_key)
		row.append(None)
		return (len(column_keys) -1)

def resetRow(row):

	for items in row:
		items = None

def getTimeString():
	current_time = time.localtime(time.time())
	day = current_time.tm_mday
	hour = current_time.tm_hour
	minute = current_time.tm_min
	second = current_time.tm_sec
	time_string = str(day) + "/" + str(hour) + ":" + str(minute) + ":" + str(second)
	return time_string

def line_appender(file, line):
    file.write(line + "\n")

if __name__ == '__main__':
	main()	