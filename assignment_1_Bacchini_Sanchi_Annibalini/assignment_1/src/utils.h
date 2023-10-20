#include <TimerOne.h>
#include <avr/sleep.h>
#include <avr/power.h>
#include <EnableInterrupt.h>
#define N_LED 4
#define LED_PIN1 13
#define LED_PIN2 12
#define LED_PIN3 11
#define LED_PIN4 10
#define LED_ERRORPIN 9
#define BUTTON_PIN1 7
#define BUTTON_PIN2 6
#define BUTTON_PIN3 5
#define BUTTON_PIN4 4
#define POT_PIN A0
#define N_LED 4
#define FIXAMOUNT 300
#define T1 1
#define T2 4
#define T3 5

int score;
int *turnedOffOrder = (int[]){0, 0, 0, 0};
int *pressedOrder = (int[]){0, 0, 0, 0};
int pos;
float factor;
unsigned long t2;
unsigned long t3;
unsigned long milliSecondsMultiplier;
unsigned long microsecondMultiplier;
unsigned long prevoiusTime;
int turnedOffLed;
int fadeAmount;
int brightness;
volatile enum gameState {
    preGame,
    inGame,
    outGame,
    endGame,
} gameState;
int times;

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

/**
 * Function to check if the order of pressed buttons is right
 */
void checkPressOrder()
{
    for (int x = 0; x < N_LED; x++)
    {
        if (pressedOrder[x] != turnedOffOrder[3 - x])
        {
            gameState = endGame;
        }
    }
    pos = 0;
}

/**
 * Function to insert the pressed button into the array
 */
void insertButton(int n)
{
    // check for bouncing of phisical buttons
    if (millis() - prevoiusTime > FIXAMOUNT)
    {
        pressedOrder[pos] = n;
        Serial.println(pressedOrder[pos]);
        prevoiusTime = millis();
        pos++;
    }
}

/**
 * Function for button 1 interrupt
 */
void button1pressed()
{
    insertButton(1);
}

/**
 * Function for button 2 interrupt
 */
void button2pressed()
{
    insertButton(2);
}

/**
 * Function for button 3 interrupt
 */
void button3pressed()
{
    insertButton(3);
}

/**
 * Function for button 4 interrupt
 */
void button4pressed()
{
    insertButton(4);
}

/**
 * function to end the game after the timeout
 */
void goToEndGame()
{
    gameState = endGame;
    pos = 0;
}

void startGame()
{
    stopTimer();
    times = 0;
    prevoiusTime = millis();
    digitalWrite(LED_ERRORPIN, LOW);
    disableInterrupt(BUTTON_PIN1);
    disableInterrupt(BUTTON_PIN2);
    disableInterrupt(BUTTON_PIN3);
    disableInterrupt(BUTTON_PIN4);
    enableInterrupt(BUTTON_PIN1, button1pressed, CHANGE);
    enableInterrupt(BUTTON_PIN2, button2pressed, CHANGE);
    enableInterrupt(BUTTON_PIN3, button3pressed, CHANGE);
    enableInterrupt(BUTTON_PIN4, button4pressed, CHANGE);
    gameState = outGame;
}

void sleep()
{
    set_sleep_mode(SLEEP_MODE_PWR_DOWN);
    sleep_enable();
    power_adc_disable();
    power_spi_disable();
    power_timer0_disable(); // only timer 1 to
    power_timer2_disable(); // not reinitialize it
    power_twi_disable();
    sleep_mode();
    // in this point arduino wake up
    Serial.println("wake up");
    sleep_disable();
    power_all_enable();
}

void enterPreGame()
{
    disableInterrupt(BUTTON_PIN1);
    enableInterrupt(BUTTON_PIN1, startGame, CHANGE);
    Serial.println("Welcome to the Restore the Light Game. Press Key B1 to Start");
    // set timer to deepSleep
    Timer1setPeriod(goToSleep, 5 * microsecondMultiplier);
}

void wakeUp()
{
    Serial.println("svegliato");
}

void goToSleep()
{
    times++;
    if (times == 2)
    {
        // attacca gli interrupt per svegliare e vai a dormire
        disableInterrupt(BUTTON_PIN1);
        disableInterrupt(BUTTON_PIN2);
        disableInterrupt(BUTTON_PIN3);
        disableInterrupt(BUTTON_PIN4);
        enableInterrupt(BUTTON_PIN1, wakeUp, CHANGE);
        enableInterrupt(BUTTON_PIN2, wakeUp, CHANGE);
        enableInterrupt(BUTTON_PIN3, wakeUp, CHANGE);
        enableInterrupt(BUTTON_PIN4, wakeUp, CHANGE);
        sleep();
        times = 0;
        enterPreGame();
    }
    else
    {
        Serial.println("ciao");
        Timer1setPeriod(goToSleep, 5 * microsecondMultiplier);
    }
}