#include <TimerOne.h>

void setup(){
    // inizialize serial monitor, leds and buttons
    Serial.begin(9600);
    delay(2000);
    createTimer();
}

void loop() {


}

/**
 * function to end the game after the timeout
 */
void goToEndGame()
{
    Serial.println("fine");
}

/**
 * function to create a timer and attach the interrupt
 */
void createTimer()
{
    TCCR1A = 0;
    TCCR1B = 0;
    TCNT1 = 0;
    Timer1.initialize();
    Timer1.setPeriod(5 * 1000000);
    Timer1.attachInterrupt(goToEndGame);
}