#include <TimerOne.h>

void setup()
{
    // inizialize serial monitor, leds and buttons
    Serial.begin(9600);
    delayMicroseconds(5000000);
    createTimer();
}

void loop()
{
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
    Timer1.initialize(5 * 1000000);
    Timer1.attachInterrupt(goToEndGame);
}