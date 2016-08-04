# smart-bike-light makefile
# adapted from a VUSB example project's makefile
# matt ruffner 2016

DEVICE  = attiny84
F_CPU   = 1000000
FUSE_L  = 0x62
FUSE_H  = 0xd7
AVRDUDE = avrdude -c usbtiny -P usb -p $(DEVICE)

CFLAGS  = -Iusbdrv -I. -DDEBUG_LEVEL=0
OBJECTS =  sbl.o adxl362.o

COMPILE = avr-gcc -Wall -Os -DF_CPU=$(F_CPU) $(CFLAGS) -mmcu=$(DEVICE)
COMPILEPP = avr-g++ -Wall -Os -DF_CPU=$(F_CPU) $(CFLAGS) -mmcu=$(DEVICE)

# symbolic targets:
help:
	@echo "This Makefile has no default rule. Use one of the following:"
	@echo "make hex ....... to build the firmware"
	@echo "make program ... to flash fuses and firmware"
	@echo "make fuse ...... to flash the fuses"
	@echo "make flash ..... to flash the firmware"
	@echo "make clean ..... to delete objects and hex file"

hex: sbl.hex

program: flash fuse

# rule for programming fuse bits:
fuse:
	@[ "$(FUSE_H)" != "" -a "$(FUSE_L)" != "" ] || \
                { echo "*** Edit Makefile and choose values for FUSE_L and FUSE_H!"; exit 1; }
	$(AVRDUDE) -U hfuse:w:$(FUSE_H):m -U lfuse:w:$(FUSE_L):m
read:
	$(AVRDUDE) -U hfuse:r:high.txt:r -U lfuse:r:low.txt:r

# rule for uploading firmware:
flash: sbl.hex
	$(AVRDUDE) -U flash:w:sbl.hex:i

# rule for deleting dependent files (those which can be built by Make):
clean:
	rm -f *.o *.hex *.elf

# Generic rule for compiling C files:
.c.o:
	$(COMPILE) -c $< -o $@

# Generic rule for assembling Assembler source files:
.S.o:
	$(COMPILE) -x assembler-with-cpp -c $< -o $@
# "-x assembler-with-cpp" should not be necessary since this is the default
# file type for the .S (with capital S) extension. However, upper case
# characters are not always preserved on Windows. To ensure WinAVR
# compatibility define the file type manually.

# Generic rule for compiling C to assembler, used for debugging only.
.c.s:
	$(COMPILE) -S $< -o $@

# file targets:

# Since we don't want to ship the driver multipe times, we copy it into this project:
#usbdrv:
#	cp -r ../../../usbdrv .

sbl.elf: $(OBJECTS)	# usbdrv dependency only needed because we copy it
	$(COMPILE) -o sbl.elf $(OBJECTS)

sbl.hex: sbl.elf
	rm -f sbl.hex sbl.eep.hex
	avr-objcopy -j .text -j .data -O ihex sbl.elf sbl.hex
	avr-size sbl.hex

# debugging targets:

disasm:	sbl.elf
	avr-objdump -d sbl.elf