/*
 * Pin Change Interrupt Example
 * Author: Alex from Inside Gadgets (http://www.insidegadgets.com)
 *
 */
 
#include <avr/sleep.h>
#include <avr/power.h>
#include <Adafruit_NeoPixel.h>



#ifndef cbi
#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#endif
#ifndef sbi
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#endif
// From http://interface.khm.de/index.php/lab/experiments/sleep_watchdog_battery/
//cbi(ADCSRA,ADEN); // Switch Analog to Digital converter OFF
//sbi(ADCSRA,ADEN);  // Switch Analog to Digital converter ON

uint8_t dPinLedPwr = 3;
uint8_t dPinLedData = 4;
uint8_t aPinPwr = 1;
uint8_t aPinLdr = 0;

uint16_t pwrMin = 1000;
uint16_t ldrMin = 800;
bool sleeping = true;
bool powered  = false;
bool running = false;
int numCycles = 0;

Adafruit_NeoPixel strip = Adafruit_NeoPixel(2, dPinLedData, NEO_GRB + NEO_KHZ800);

void setup(){
  pinMode(dPinLedData,INPUT);
  pinMode(dPinLedPwr,INPUT);
  pinMode(aPinLdr,INPUT);
  pinMode(aPinPwr,INPUT); //A1
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
  strip.setBrightness(100); 
 // sbi(GIMSK,PCIE);   // Turn on Pin Change interrupt
 // sbi(PCMSK,PCINT1); // Which pins are affected by the interrupt
}

void loop(){
//  if (sleeping)
//    system_sleep();
//
//  if (analogRead(aPinPwr) >= pwrMin)
//    powered = true;
//  else
//  {
//    sleeping = true;
//    powered = false;
//  }
  
  //while( (!sleeping)&&(powered) )
  while(1)
  {
    rainbow(20);
    rainbowCycle(20);
    theaterChaseRainbow(50);
    //if (analogRead(aPinPwr) < pwrMin)
    //  powered = false;
    //if (analogRead(aPinLdr) < ldrMin)
    //  sleeping = true;
    //numCycles++;
    //if(numCycles == 1000)
    //  break;
  }
  numCycles = 0;
}

void rainbow(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256; j++) {
    for(i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel((i+j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycle(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
    for(i=0; i< strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

//Theatre-style crawling lights with rainbow effect
void theaterChaseRainbow(uint8_t wait) {
  for (int j=0; j < 256; j++) {     // cycle all 256 colors in the wheel
    for (int q=0; q < 3; q++) {
        for (int i=0; i < strip.numPixels(); i=i+3) {
          strip.setPixelColor(i+q, Wheel( (i+j) % 255));    //turn every third pixel on
        }
        strip.show();
       
        delay(wait);
       
        for (int i=0; i < strip.numPixels(); i=i+3) {
          strip.setPixelColor(i+q, 0);        //turn every third pixel off
        }
    }
  }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  if(WheelPos < 85) {
   return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  } else if(WheelPos < 170) {
   WheelPos -= 85;
   return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else {
   WheelPos -= 170;
   return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
}

void system_sleep() {
  pinMode(dPinLedPwr, INPUT);
  pinMode(dPinLedData, INPUT);
  power_adc_disable();
  power_usi_disable();
  power_timer0_disable();
  power_timer1_disable();
  set_sleep_mode(SLEEP_MODE_PWR_DOWN); // Set sleep mode
  sbi(PCMSK,PCINT1); // Which pins are affected by the interrupt
  sbi(GIMSK,PCIE); // Turn off Pin Change interrupt
  sleep_mode(); // System sleeps here
}

ISR(PCINT0_vect) {
  if(!running) 
  {
    running = true;
    power_adc_enable();
    power_timer0_enable();
    power_timer1_enable();
    pinMode(dPinLedPwr, OUTPUT);
    pinMode(dPinLedData, OUTPUT);
    digitalWrite(dPinLedPwr, HIGH);
    sleeping = false;
    cbi(PCMSK,PCINT1); // Which pins are affected by the interrupt
    cbi(GIMSK,PCIE); // Turn off Pin Change interrupt
  }
}
