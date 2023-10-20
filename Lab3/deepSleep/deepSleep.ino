#include <avr/sleep.h>
#include <avr/power.h>
#include <EnableInterrupt.h>

void wakeUp(){}

void setup(){
  Serial.begin(9600);
  pinMode(2,INPUT);
  enableInterrupt(4, wakeUp, CHANGE);
  enableInterrupt(5, wakeUp, CHANGE);
  //attachInterrupt(digitalPinToInterrupt(2), wakeUp, RISING);
}

void loop(){
  analogWrite(3, 255);
  Serial.println("GOING IN POWER DOWN IN 1s ...");
  Serial.flush();
  delay(1000);
  
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  sleep_enable();
  power_adc_disable();
  power_spi_disable();
  power_timer0_disable(); // only timer 1
  power_timer2_disable(); // on
  power_twi_disable();
  sleep_mode();
  /** The program will continue from here. **/
  Serial.println("WAKE UP");
  /* First thing to do is disable sleep. */
  sleep_disable();
  power_all_enable();
}