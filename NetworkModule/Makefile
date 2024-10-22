# Create the build directory
$(BUILD_DIR):
	mkdir $(BUILD_DIR)

led:
# 	build
	avr-gcc -mmcu=attiny841 -Wall -Os -o build/LEDmodule.elf src/Modules/LEDmodule.c
	avr-objcopy -j .text -j .data -O ihex build/LEDmodule.elf build/LEDmodule.hex
#	upload
	avrdude -c avrisp -p t841 -P COM5 -U flash:w:build/LEDmodule.hex

servo:
# 	build
	avr-gcc -mmcu=attiny841 -Wall -Os -o build/SERVOmodule.elf src/Modules/SERVOmodule.c
	avr-objcopy -j .text -j .data -O ihex build/SERVOmodule.elf build/SERVOmodule.hex
#	upload
	avrdude -c avrisp -p t841 -P COM5 -U flash:w:build/SERVOmodule.hex