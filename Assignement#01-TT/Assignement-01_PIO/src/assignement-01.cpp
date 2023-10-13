/**
 * @author tommaso.severi2@studio.unibo.it
 * @author tommaso.ceredi@studio.unibo.it
 * @brief Restore the lights minigame
 * @version 1.0
 */

// Standard arduino library
#include <Arduino.h>
// Standard C libraries
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
// Enables all pins to handle interrupts
#include <EnableInterrupt.h>
// Needed to use Timer1
#include <TimerOne.h>
// Needed to put arduino to sleep
#include <avr/sleep.h>

// Number of buttons and leds couples used
#define COUPLES 4
// Pin for the red led
#define REDLED 6
// Pins for the green leds
#define GREENLED1 12
#define GREENLED2 11
#define GREENLED3 10
#define GREENLED4 9
// Pins for the buttons
#define BUTTON1 5
#define BUTTON2 4
#define BUTTON3 3
#define BUTTON4 2
// Pin for the potentiometer
#define POTENTIOMETER A0
// How many moments it's needed to take track of time
#define TIMERS 3
// Minum time given fro an action
#define MIN_TIME 1000
// Maximum time given for an action
#define MAX_TIME 3000
// The difference in time from one action to the next
#define TIME_INCREASE 2000
// Quantity for how much the led shifts in light while fading
#define FADE 5
// Simbolic for variables waiting to have a valid number assigned
#define UNDEFINED -1
// Frequently used value when waiting
#define DELAY 1000
// Time before going to sleep
#define SLEEP_TIMER 10 * 1000000
/* This are not standard potentiometer values but
    the one we are using is of really poor quality */
#define POT_MIN 23
#define POT_MAX 1001
// Levels of difficulty of the game
#define MIN_DIFF 1
#define MAX_DIFF 4
// Time before getting another try
#define MISTAKE_TIME DELAY*10

/**
 * This struct couples all the buttons with their respective leds. 
 * Also maintains their turn in the game.
*/
typedef struct {
    int buttonPin;
    int ledPin;
    int turn;
} buttonLed;

/**
 * This is used to define the states the game can assume.
*/
typedef enum {
    settingUp,
    starting,
    showingOrder,
    waitingPlayer,
    madeMistake,
    sleeping
} gameState;

/**
 * t1 [0] is the time elapsed to start the game
 * t2 [1] is time elapsed to turn on the leds
 * t3 [2] is the time to wait for the player
*/
int times[TIMERS];

/**
 * The main function executed before everything else 
 * used to set up the variables.
*/
void setup();

/**
 * The main function that handles all the program while looping.
*/
void loop();

/**
 * Sets the state of the game.
 * @param newState the new state the game should assume.
*/
void setState(gameState newState);

/**
 * Attaches a function to the buttons' interrupt.
*/
void enableButtonsInterrupt(interruptFunctionType function);

/**
 * Disables the buttons' interrupt.
*/
void disableButtonsInterrupt();

/**
 * Sets the timer one to call the function f after time microseconds.
*/
void setTimerOne(unsigned long time, void (*f)());

/**
 * Resets the timer one.
*/
void resetTimerOne();

/**
 * Restarts the timer one. 
 */
void restartTimerOne(unsigned long time, void (*f)());

/**
 * Waits for the player to press the button to start the game.
*/
void waitingStart();

/**
 * Starts the game.
*/
void startGame();

/**
 * Handles the time out of the time given to player.
*/
void timeOut();

/**
 * Handles the resetting of the game.
*/
void restart();

/**
 * Switches off the leds in the reverse order they should be pressed.
*/
void startTurningOffLeds();

/**
 * Handles the button presses.
*/
void buttonPressed();

/**
 * Switches on or off all the green leds.
 * @param state false is they should be turned off, true otherwise.
*/
void switchGreens(bool state);

/**
 * Switches every led off.
*/
void switchOff();

/**
 * Generates a random order for the leds.
 * @param arr the array maintaining each led turn.
*/
int randomLedOrder(int *arr);

/**
 * Generates a random integer between min and max.
 * @param min the minimum value the integer can assume.
 * @param max the maximum value the integer can assume.
*/
int randomInt(int min, int max);

/**
 * Generates a random float between min and max.
 * @param min the minimum value the float can assume.
 * @param max the maximum value the float can assume.
*/
float randomFloat(float min, float max);

/**
 * Checks if a number is present in an array.
*/
bool isPresent(int *arr, int num);

/**
 * Generates random times for the game.
*/
void generateTimes();

/**
 * Sets the times for the game descreasing them 
 * basing on the difficulty level.
*/
void setTimes();

/**
 * Gets the arduino to sleep.
*/
void sleep();

/**
 * Wakes up the arduino.
*/
void wakeUp();

void setUpStart();

// The state the game is currently in
volatile gameState state;
// The led that should be lit next while playing
volatile int currentTurn;
// Difficulty factor
volatile float difficulty;
// Used to maintain the current time to avoid buttons bouncing
volatile long prevts;
buttonLed buttonLedArr[COUPLES];
float score;
int potVal;
int brightness;
int fadeAmount;

void setup() {
    Serial.begin(115200);
    state = settingUp;
    currentTurn = 0;
    score = 0;
    prevts = 0;
    difficulty = MIN_DIFF;
    generateTimes();
    srand(micros());
    buttonLedArr[0] = { BUTTON1, GREENLED1, UNDEFINED };
    buttonLedArr[1] = { BUTTON2, GREENLED2, UNDEFINED };
    buttonLedArr[2] = { BUTTON3, GREENLED3, UNDEFINED };
    buttonLedArr[3] = { BUTTON4, GREENLED4, UNDEFINED };
    for (int i = 0; i < COUPLES; i++) {
        pinMode(buttonLedArr[i].buttonPin, INPUT);
        pinMode(buttonLedArr[i].ledPin, OUTPUT);
    }
    pinMode(REDLED, OUTPUT);
}

void loop() {
    switch (state) {
        case settingUp:
            setUpStart();
            break;
        case starting:
            waitingStart();
            break;
        case showingOrder:
            startTurningOffLeds();
            break;
        case waitingPlayer:
            break;
        case madeMistake:
            Serial.println("You lost");
            restart();
            break;
        case sleeping:
            break;
    }
}

void setState(gameState newState) {
    noInterrupts();
    state = newState;
    interrupts();
}

void enableButtonsInterrupt(interruptFunctionType function) {
    for (int i = 0; i < COUPLES; i++) {
        enableInterrupt(buttonLedArr[i].buttonPin, function, RISING);
    }
}

void disableButtonsInterrupt() {
    for (int i = 0; i < COUPLES; i++) {
        disableInterrupt(buttonLedArr[i].buttonPin);
    }
}

void setTimerOne(unsigned long time, void (*f)()) {
    Timer1.initialize();
    Timer1.setPeriod(time);
    Timer1.attachInterrupt(f);
}

void resetTimerOne() {
    Timer1.stop();
    Timer1.detachInterrupt();
}

void restartTimerOne(unsigned long time, void (*f)()) {
    resetTimerOne();
    setTimerOne(time, f);
}

void setUpStart() {
    potVal = 0;
    brightness = 0;
    fadeAmount = FADE;
    Serial.println("Welcome to the Restore the Light Game. Press Key B1 to Start");
    enableInterrupt(buttonLedArr[0].buttonPin, startGame, RISING);
    setTimerOne(SLEEP_TIMER, sleep);
    setState(starting);
}

void waitingStart() {
    int tmpPotVal = map(analogRead(POTENTIOMETER), POT_MIN, POT_MAX, MIN_DIFF, MAX_DIFF);
    if (potVal != tmpPotVal) {
        restartTimerOne(SLEEP_TIMER, sleep);
        potVal = tmpPotVal;
    }
    analogWrite(REDLED, brightness);
    brightness = brightness + fadeAmount;
    if (brightness == 0 || brightness == 255) {
        fadeAmount = -fadeAmount;
    }
    delay(30);
}

void startGame() {
    long ts = millis();
    if (ts - prevts > 40) {
        prevts = ts;
        setState(showingOrder);
    }
}

void timeOut() {
    setState(madeMistake);
}

void restart() {
    score = 0;
    disableButtonsInterrupt();
    switchOff();
    generateTimes();
    resetTimerOne();
    digitalWrite(REDLED, HIGH);
    delay(DELAY);
    digitalWrite(REDLED, LOW);
    delay(MISTAKE_TIME);
    setState(settingUp);
}

void startTurningOffLeds() {
    analogWrite(REDLED, 0);
    resetTimerOne();
    noInterrupts();
    difficulty = 1 - (potVal/10);
    setTimes();
    interrupts();
    switchGreens(true);
    delay(times[0]);
    int arr[COUPLES];
    for (int i = 0; i < COUPLES; i++) {
        arr[i] = UNDEFINED;
        buttonLedArr[i].turn = UNDEFINED;
    }
    for (int i=0; i<COUPLES; i++) {
        delay(times[1]/4);
        arr[i] = randomLedOrder(arr);
        digitalWrite(buttonLedArr[arr[i]].ledPin, LOW);
        buttonLedArr[arr[i]].turn = i;
    }
    currentTurn = COUPLES-1;
    setTimerOne(times[2] * 1000000, timeOut);
    enableButtonsInterrupt(buttonPressed);
    setState(waitingPlayer);
}

void buttonPressed() {
    noInterrupts();
    long ts = millis();
    if (ts - prevts > 40) {
        prevts = ts;
        for (int i = 0; i < COUPLES; i++) {
            if (digitalRead(buttonLedArr[i].buttonPin) == HIGH && buttonLedArr[i].turn != UNDEFINED) {
                if (buttonLedArr[i].turn == currentTurn) {
                    currentTurn--;
                    buttonLedArr[i].turn = UNDEFINED;
                    digitalWrite(buttonLedArr[i].ledPin, HIGH);
                    if (currentTurn < 0) {
                        score+=difficulty+1;
                        Serial.print("New point! Score: ");
                        Serial.println(score);
                        setTimes();
                        state = showingOrder;
                    }
                } else {
                    state = madeMistake;
                }
                break;
            }
        }
    }
    interrupts();
}

void switchGreens(bool state) {
    for (int i = 0; i < COUPLES; i++) {
        digitalWrite(buttonLedArr[i].ledPin, state ? HIGH : LOW);
    }
}

void switchOff() {
    switchGreens(false);
    digitalWrite(REDLED, LOW);
    Serial.flush();
}

int randomLedOrder(int *arr) {
    int randomNumber = randomInt(0, COUPLES-1);
    while (isPresent(arr, randomNumber)) {
        randomNumber = randomInt(0, COUPLES-1);
    }
    return randomNumber;
}

int randomInt(int min, int max) {
    return (rand() % (max - min + 1)) + min;
}

float randomFloat(float min, float max) {
    return ((float)rand() / (float)RAND_MAX) * (max - min) + min;
}

bool isPresent(int *arr, int num) {
    for (int i = 0; i < COUPLES; i++) {
        if (arr[i] == num) {
            return true;
        }
    }
    return false;
}

void generateTimes() {
    for (int i = 0; i < TIMERS; i++) {
        times[i] = (int)randomFloat((float)MIN_TIME, (float)MAX_TIME);
    }
}

void setTimes() {
    for (int i = 0; i < TIMERS; i++) {
        times[i] *= difficulty;
    };
    Serial.print("Times: ");
    Serial.print(times[2]);
}

void sleep() {
    Serial.println("GOING TO POWER DOWN IN 1 SECOND...");
    delay(DELAY);
    switchOff();
    setState(sleeping);
    set_sleep_mode(SLEEP_MODE_PWR_DOWN);
    sleep_enable();
    enableButtonsInterrupt(wakeUp);
    sleep_mode();
    sleep_disable();
    disableButtonsInterrupt();
}

void wakeUp() {
    prevts = millis();
    state = settingUp;
}