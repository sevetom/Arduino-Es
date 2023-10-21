#include "utils.h"
#include "Arduino.h"
#define N_LED 4

void randomizeOrder(int turnedOffOrder[])
{
    randomSeed(analogRead(4));
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

void flushArray(int *array)
{
    array[0] = 0;
    array[1] = 0;
    array[2] = 0;
    array[3] = 0;
}

