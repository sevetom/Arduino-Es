#define N_LED 4
#include <TimerOne.h>
/**
 * Function to create a pseudo-random order to turn off leds
 */
void randomizeOrder(int turnedOffOrder[])
{
    int i = 1;
    while (i <= N_LED)
    {
        int choise = random(0, N_LED);
        if (turnedOffOrder[choise] == 0)
        {
            turnedOffOrder[choise] = i;
            i++;
        }
    }
    // print the order on serial line
    Serial.println(turnedOffOrder[0]);
    Serial.println(turnedOffOrder[1]);
    Serial.println(turnedOffOrder[2]);
    Serial.println(turnedOffOrder[3]);
}

/**
 * Function to flush an array of 4 elements
 */
void flushArray(int *array)
{
    array[0] = 0;
    array[1] = 0;
    array[2] = 0;
    array[3] = 0;
}

/**
 * Function to initialize a timer
 */
void Timer1Initialize()
{
    noInterrupts();
    Timer1.initialize();
    Timer1.stop();
    interrupts();
}

/**
 * Function to set a period and a function interrupt to the timer
 */
void Timer1setPeriod(void (*isr)(), unsigned long microseconds)
{
    noInterrupts();
    Timer1.attachInterrupt(isr, microseconds);
    interrupts();
}

/**
 * funtcion to stop the timer and detach the interrupt
 */
void stopTimer()
{
    Timer1.stop();
    Timer1.detachInterrupt();
}
