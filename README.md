FitFullBoard is a Raspberry Pi- and Raspberry Pi Zero-compatible daughter board intended to power up and power down the the RPi board in sync with outside signals. 

I use it to keep an RPi media player powered along with my television. I connect the signal pin to a USB cable which I plug into my smart teevee (but I power the RPi from an independent power source.) When the teevee comes on, it brings its USB port on, which wakes up the RPi. When the teevee goes off, it brings its USB port down, which puts the RPi to sleep (following a configurable grace period in case you turn the teevee back on).  It is possible to turn the teevee back on at just the wrong time so as to get them out of sync (after the grace period expires, while the RPi operating system is still shutting down). 

FitFullBoard uses an ATtiny85 to control power up along with GPIO5 / physical pin 5, and python code running in the RPi to handle shutdown and grace period.
