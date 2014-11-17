/*
 * Pin Change Interrupt Example
 * Version: 1.0
 * Author: Alex from Inside Gadgets (http://www.insidegadgets.com)
 * Created: 25/02/2011
 *
 * Demonstration of the pin change interrupt
 * LED on digital pin 0
 * Interrupt on digital pin 1
 * 10k resistor on digital pin 1 to GND
 *
 */
 
#include <avr/sleep.h>

#ifndef cbi
#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#endif
#ifndef sbi
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#endif

uint8_t dPinLed = 4;
uint8_t aPinPwr = 1;
uint8_t aPinLdr = 0;
uint16_t threshold = 1000;

void setup(){
  pinMode(dPinLed,OUTPUT);
  pinMode(aPinLdr,INPUT);
  pinMode(aPinPwr,INPUT); //A1
  
  sbi(GIMSK,PCIE);   // Turn on Pin Change interrupt
  sbi(PCMSK,PCINT1); // Which pins are affected by the interrupt
}

int value = 0;
bool sleeping = true;
bool powered  = false;

void loop(){
  if (sleeping)
  {
    system_sleep();
    //sleeping = false;
  }

  value = analogRead(aPinPwr);
  if (value >= threshold)
    powered = true;
  else
  {
    sleeping = true;
    powered = false;
  }

  while( (!sleeping)&&(powered) )
  {
    digitalWrite(dPinLed,HIGH); 
    delay(1000);
    digitalWrite(dPinLed,LOW);
    delay(1000);
    //cbi(PCMSK,PCINT1); // Which pins are affected by the interrupt
    //cbi(GIMSK,PCIE); // Turn off Pin Change interrupt
  }
}

// From http://interface.khm.de/index.php/lab/experiments/sleep_watchdog_battery/
void system_sleep() {
  cbi(ADCSRA,ADEN); // Switch Analog to Digital converter OFF
  set_sleep_mode(SLEEP_MODE_PWR_DOWN); // Set sleep mode
  sleep_mode(); // System sleeps here
  sbi(ADCSRA,ADEN);  // Switch Analog to Digital converter ON
}

ISR(PCINT0_vect) {
  sleeping = !sleeping;
}

