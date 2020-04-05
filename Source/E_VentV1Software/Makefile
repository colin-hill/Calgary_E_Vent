.PHONY: all
all: libraries compile

.PHONY: compile
compile:
	arduino-cli compile --fqbn arduino:avr:mega:cpu=atmega2560 --warnings all

.PHONY: arduino_core_avr
arduino_core_avr:
	arduino-cli core update-index
	arduino-cli core install arduino:avr

.PHONY: LiquidCrystal
LiquidCrystal: arduino_core_avr
	arduino-cli lib install LiquidCrystal

.PHONY: libraries
libraries: LiquidCrystal
