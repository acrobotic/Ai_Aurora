#include <avr/power.h>
#include <avr/sleep.h>

int ledPin = 4;

void setup() {
    pinMode(ledPin,OUTPUT);
    power_timer0_disable();    // Disable Timer 0 (affects delay function)
    power_timer1_disable();    // Disable Timer 1
    power_adc_disable();       // to save power
    PCMSK |= _BV(PCINT1);      // Set change mask for pin 1
}

bool awaken = false; 

void loop() {
  if(!awaken) {             // If last cycle...
    // Execution resumes here on wake.
    GIMSK = _BV(PCIE);     // Enable pin change interrupt
    power_all_disable();   // All peripherals off
    set_sleep_mode(SLEEP_MODE_PWR_DOWN);
    sleep_enable();
    sei();                 // Keep interrupts disabled
    digitalWrite(ledPin,HIGH);
    awaken = true;
    sleep_mode();          // Power down CPU (pin 1 will wake)
    GIMSK = 0;             // Disable pin change interrupt
    PCMSK = 0;             // Disable pin change interrupt
  } else {
    power_timer0_enable(); // Re-enable timer
    //power_all_enable();    // Re-enable USI
    digitalWrite(ledPin,LOW);
    delay(1000);
    digitalWrite(ledPin,HIGH);
    delay(1000);
  }

}
