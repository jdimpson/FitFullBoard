FitFullBoard is a Raspberry Pi- and Raspberry Pi Zero-compatible daughter board intended to power up and power down the the RPi board in sync with outside signals. 

I use it to keep an RPi media player powered along with my television. I connect the signal pin to a USB cable which I plug into my smart teevee (but I power the RPi from an independent power source.) When the teevee comes on, it brings its USB port on, which wakes up the RPi. When the teevee goes off, it brings its USB port down, which puts the RPi to sleep (following a configurable grace period in case you turn the teevee back on).  It is possible to turn the teevee back on at just the wrong time so as to get them out of sync (after the grace period expires, while the RPi operating system is still shutting down). 

FitFullBoard uses an ATtiny85 to control power up, and python code running in the RPi to handle shutdown and grace period.

Raspberry Pi Pins used:
- GPIO3/Phys5/SCL is used to power up the RPi. This only works if the RPi has been shut down via the "poweroff" or "shutdown" Linux commands. THe board itself must still have power.
- GPIO17/Phy11 is connected to a momentary / tactile switch. Available for arbitrary software control (typically running my powerboard.py and associated code).
- GPIO27/Phy13 is connected to a resistor and then an LED. Available for arbitrary software control (e.g. flashing to show grace period, and typically running my powerboard.py and associated code).
- GPIO26/Phy37 is connected to via transistor and appropriate hold up resistor to the LBO pin of the ATtiny85. This is how the Raspberry Pi knows to shut down. This is labelled LBO and "Low Battery" in the schematic because the circuit any my lowbattery code were reused from one's that expect a signal when the Li Ion battery level is below 3.4VDC (or whatever) (typically running my powerboard.py and associated code).

ATtiny85 pin used:
- PB0/MOSI used as RX for soft UART (for debugging)
- PB1/MISO used as TX for soft UART (for debugging)
- PB2/SCK used to tell RPi to power off.
- PB3 used to receive EXTernal signal (e.g. teevee power on indication). Circuit can be held up to 5VDC or down to ground, depending on which pads you put the resistor on (DON'T ATTACH BOTH R4 and R5 RESISTORS).
- PB4 is attached to a momentary / tactile button to allow manual start of the RPi independent of the EXTernal signal.

ATtiny85 code works with Arduino IDE and the https://github.com/SpenceKonde/ATTinyCore library.

Python code will be posted and linked to soon.
