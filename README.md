FitFullBoard is a Raspberry Pi- and Raspberry Pi Zero-compatible daughter board intended to power up and power down the the RPi board in sync with outside signals. 

I use it to keep an RPi media player powered along with my television. I connect the fitfullboard's signal pin called EXTERNAL to a USB cable which I plug into my smart teevee (but I power the RPi from an independent power source.) When the teevee comes on, it brings its USB port on, which wakes up the RPi. When the teevee goes off, it brings its USB port down, which puts the RPi to sleep (following a configurable grace period).  It is possible to turn the teevee back on at just the wrong time so as to get them out of sync (after the grace period expires, while the RPi operating system is still shutting down). 

FitFullBoard uses an ATtiny85 to control power up / power down and to monitor the EXTERNAL signal, and python code running in the RPi to handle shutdown and grace period. The grace period works because the ATtiny will hold the shutdown signal to the RPI down indefinitely. If the EXTERNAL signal comes back on, the ATtiny with bring the shutdown signal back high again. If this happens within the grace period, the python software will not initiate shutdown. You can instead use the Linux kernel's builtin signal handling for shutdown, althuogh AFAIK it has no way of offering a grace period.

Raspberry Pi Pins used:
- GPIO3/Phys5/SCL is used to power up the RPi. This only works if the RPi has been shut down via the "poweroff" or "shutdown" Linux commands. THe board itself must still have power.
- GPIO17/Phy11 is connected to a momentary / tactile switch. Available for arbitrary software control (typically running my powerboard.py and associated code).
- GPIO27/Phy13 is connected to a resistor and then an LED. Available for arbitrary software control (e.g. flashing to show grace period, and typically running my powerboard.py and associated code).
- GPIO26/Phy37 is connected via transistor and appropriate hold up resistor to the LBO/Shutdown pin of the ATtiny85. This is how the Raspberry Pi knows to shut down. This is labelled LBO and referred to as "Low Battery" in the schematic because the circuit any my python lowbattery code were reused from the PowerBoard (https://github.com/jdimpson/PowerBoard) and PowerHolder (https://github.com/jdimpson/PowerHolder) projects one's that expect a signal when the Li Ion battery level is below 3.4VDC (or whatever) (typically running my powerboard.py and associated code).

ATtiny85 pin used:
- PB0/MOSI used as RX for soft UART (for debugging, note the arduino code doesn't RX currently)
- PB1/MISO used as TX for soft UART (for debugging)
- PB2/SCK used to send the LBO/shutdown signal that tells the RPi to power off following a grace period.
- PB3 used to receive EXTERNAL signal (e.g. teevee power on indication). Circuit can be held up to 5VDC or down to ground, depending on which pads you put the resistor on (DON'T ATTACH BOTH R4 and R5 RESISTORS).
- PB4 is attached to a momentary / tactile button to allow manual start of the RPi independent of the EXTernal signal.

Other items of Note:
- I honestly don't remember why I called this the FitFullBoard. Something about the phrase "sleeping in fits and starts" being an analogue to automatically starting up and shutting down in response to an external influence. But honestly who knows? At one point I called it GraceFullBoard because it allowed for graceful shutdown of a raspberry pi in response to a signal. I should have kept that name.
- ATtiny85 code works with Arduino IDE and requires the https://github.com/SpenceKonde/ATTinyCore library.
- Python code referenced above will be posted and linked to soon.
- The use of GPIO3 to cause a shutdown RPi to restart does not seem to be well known. At least, it's not referenced on pinout.xyz (https://pinout.xyz/pinout/pin5_gpio3#). I've personally tested it to work on RPi Zero Ws, RPi 3B, and an RPi 4B with updated firmware. Note that this behaviour of GPIO3 is *different* from that of the RUN pin that is found in elsewhere on the board (not on the 40-pin header). The RUN pin will, when brought high, cause the RPi to reboot, even if the CPU is already running. This crashes the OS, which isn't something I want to do regularly. GPIO3 acts like any other pin when the CPU is running normally. It only seems to have special behaviour when the the CPU has halted following a graceful shutdown of the OS. When it gets brought high, it will cause a reboot. But *only* when the CPU was halted. See also https://www.raspberrypi.org/forums/viewtopic.php?t=113200 and https://www.raspberrypi.org/forums/viewtopic.php?p=767687#p767687
