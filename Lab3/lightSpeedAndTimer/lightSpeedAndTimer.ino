#include <avr/sleep.h>
#include <avr/power.h>
#include "Timer.h"

Timer* timer;

void setup(){
  Serial.begin(9600);
  timer = new Timer(); /* timer 1 */
  timer->setupPeriod(5000);
}

void sleep(void){
  set_sleep_mode(SLEEP_MODE_IDLE);
  sleep_enable();
  power_adc_disable();
  power_spi_disable();
  power_timer0_disable(); // only timer 1
  power_timer2_disable(); // on
  power_twi_disable();
  sleep_mode();
  /* back */
  sleep_disable();
  power_all_enable();
}

void loop(){
  Serial.println("GOING IN SLEEP");
  Serial.flush();
  sleep();
  Serial.println("WOKE UP");
}