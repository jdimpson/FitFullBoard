/**********************************************************************
 * ATTiny85 MCU monitors an external signal. When the signal (EXT) 
 * activates, MCU will bring RPi GPIO3 (START) low, which, if the RPi 
 * previously did a poweroff, will cause it to restart. At the same time, 
 * MCU will bring RPi GPIO26 (LBO) high. When the EXT signal deactivates, 
 * MCU will bring RPi GPIO26 (LBO) low.
 * 
 * The intention for the signal on GPIO26 is that the RPi runs software 
 * that treats it as a low-battery signal. When the software starts up, 
 * it looks at GPIO26. If high, it knows to use the pin as low-battery signal. 
 * (If low, don't monitor pin.) When GPIO26 goes low, RPi software will 
 * initiate a graceful shutdown.
 * 
 * Could probably do both the restart and LBO on GPIO3, but GPIO26 is used
 * for LBO due to commonality with my batter power boards.
 * 
 * Don't forget to level shift the LBO and START pins, assuming MCU is running 
 * at 5V.
 * 
 * (c) 2019 jdimpson
 **********************************************************************/

// NOTE: right now, the EXT signal must be HIGH for on, LOW for off. Would 
// be cool to use EEPROM to store the EXT signal mode, and adjust accordingly.

#include <SoftwareSerial.h>
#include <EEPROM.h>
#include "Debounce.h"

#define EXT   3 // Physical 2 / PB3 / A3
#define START 4 // Physical 3 / PB4
#define RX   -1 // Physical 5 / PB0; Reserved, not currently used. 
#define TX    1 // Physical 6 / PB1
#define LBO   2 // Physical 7 / PB2

#define OFF          0
#define STARTUP      1
#define SYNC_SIGNAL  2
#define HIGH_MONITOR 3
#define LOW_MONITOR  4

SoftwareSerial ss(RX, TX);
Debounce Button(EXT);

int state = STARTUP;
int prevstate = OFF;

void setup() {
  ss.begin(9600);
  ss.println(F("fitfullBoard"));
  ss.println(F(__FILE__));
  ss.print(F("compiled ")); 
  ss.println(__DATE__);
  delay(10);

  pinMode(EXT,INPUT);
  pinMode(LBO,OUTPUT);
  pinMode(START,INPUT); // leave high impedence

  ss.println("Waiting a second...");
  delay(1000);
  ss.println("Go!");
}

void loop() {
  state_machine(); 
}

inline void stay_awake() {
  if(digitalRead(LBO) != HIGH) {
    digitalWrite(LBO,HIGH);
  }
}

inline void wake_up() {
  pinMode(START,OUTPUT); // go low impedence
  digitalWrite(START,HIGH); // then go to high, which will switch the RPi's GPIO3 to ground via MOSFET
  delay(1000);
  pinMode(START,INPUT);// return to high impedence
}

inline void stay_sleep() {
  if(digitalRead(LBO) == HIGH) {
    digitalWrite(LBO,LOW);
  }
}

inline void stahp() {
  abort();
  // or assert(0); #include <assert.h> 
  // or set_sleep_mode(SLEEP_MODE_PWN_DOWN); cli(); sleep_mode(); #include <avr/sleep.h>
}

inline void state_machine() {
  bool delta = false;
  bool extsig = Button.read();

  if(state != prevstate) {
    delta = true;
    ss.print(prevstate);
    ss.print(F(" changed to "));
    ss.println(state);
    delay(50);
  }
  prevstate = state;
  //ss.print(F("extsig "));
  //ss.println(extsig);

  switch(state) {
    case STARTUP:
      state = SYNC_SIGNAL;
      stay_awake();
      break;

    case SYNC_SIGNAL:
      stay_awake();
      if(extsig) {
        state = HIGH_MONITOR;
      }
      break;

    case HIGH_MONITOR:
      if(delta) {
        wake_up();
      }
      stay_awake();
      if(!extsig) {
        state = LOW_MONITOR;
      }
      break;

    case LOW_MONITOR:
      stay_sleep();
      if(extsig) {
        state = HIGH_MONITOR;
      }
      break;

    default:
      ss.print("UNKNOWN STATE, ABORTING ");
      ss.println(state);
      delay(50);
      stahp();
      break;
  }

  delay(100);
}
