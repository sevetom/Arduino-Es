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
/* How many variables that are needed to take track of time
    in the differents parts of the game */
#define TIMERS 3
// Minum time given fro an action (milliseconds)
#define MIN_TIME 1000
// Maximum time given for an action (milliseconds)
#define MAX_TIME 3000
// The difference in time from one action to the next (milliseconds)
#define TIME_INCREASE 2000
// Quantity for how much the led shifts in light while fading
#define FADE 5
// Simbolic for variables waiting to have a valid number assigned
#define UNDEFINED -1
// Frequently used value when waiting (milliseconds)
#define DELAY 1000
// Time before going to sleep (microseconds)
#define SLEEP_TIME 10 * 1000000
// Time before getting another try (milliseconds)
#define MISTAKE_TIME 10 * 1000
/* This are not standard potentiometer values but
    the one we are using is of really poor quality */
#define POT_MIN 23
#define POT_MAX 1001
// Levels of difficulty of the game
#define MIN_DIFF 1
#define MAX_DIFF 4

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
void setConcurrentState(gameState newState);

/**
 * Attaches a function to the buttons' interrupt.
 * @param function the function to be attached.
*/
void enableButtonsInterrupt(interruptFunctionType function);

/**
 * Disables the buttons' interrupt.
*/
void disableButtonsInterrupt();

/**
 * Sets the timer one to call the function f after time microseconds.
 * @param time the time in microseconds.
 * @param f the function to be called.
*/
void setTimerOne(unsigned long time, void (*f)());

/**
 * Resets the timer one.
*/
void resetTimerOne();

/**
 * Restarts the timer one. 
 * @param time the time in microseconds.
 * @param f the function to be called.
 */
void restartTimerOne(unsigned long time, void (*f)());

/**
 * Checks if enough time has passed since the last button press.
*/
bool avoidButtonsBouncing();

/**
 * Handles the setting up of the game.
*/
void setupStart();

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
 * Sets up everything needed to turn off the leds in order.
*/
int* setupTurningOffLeds(); 

/**
 * Generates a random order for the leds.
 * @param arr the array maintaining each led turn.
*/
int randomLedOrder(int *arr);

/**
 * Handles the button presses.
*/
void buttonPressed();

/**
 * Handles the correct guesses of the player.
*/
void correctGuess(int index);

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
void reduceTimes();

/**
 * Gets the arduino to sleep.
*/
void sleep();

/**
 * Wakes up the arduino.
*/
void wakeUp();

// The state the game is currently in
volatile gameState state;
// The led that should be lit next while playing
volatile int currentTurn;
// Used to maintain the current time to avoid buttons bouncing
volatile long prevts;
buttonLed buttonLedArr[COUPLES];
float score;
float difficulty;
// Used to maintain the potentiometer value
int potVal;
// Used to maintain the brightness of the red led
int brightness;
// Used to maintain the amount of light the red led is shifting
int fadeAmount;

void setup() {
    state = settingUp;
    currentTurn = 0;
    score = 0;
    prevts = 0;
    difficulty = MIN_DIFF;
    srand(micros());
    generateTimes();
    buttonLedArr[0] = { BUTTON1, GREENLED1, UNDEFINED };
    buttonLedArr[1] = { BUTTON2, GREENLED2, UNDEFINED };
    buttonLedArr[2] = { BUTTON3, GREENLED3, UNDEFINED };
    buttonLedArr[3] = { BUTTON4, GREENLED4, UNDEFINED };
    for (int i = 0; i < COUPLES; i++) {
        pinMode(buttonLedArr[i].buttonPin, INPUT);
        pinMode(buttonLedArr[i].ledPin, OUTPUT);
    }
    pinMode(REDLED, OUTPUT);
    Serial.begin(115200);
}

void loop() {
    switch (state) {
        case settingUp:
            setupStart();
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
            restart();
            break;
        case sleeping:
            break;
    }
}

void setConcurrentState(gameState newState) {
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

bool avoidButtonsBouncing() {
    long ts = millis();
    if (ts - prevts > 40) {
        prevts = ts;
        return true;
    }
    return false;
}

void setupStart() {
    potVal = 0;
    brightness = 0;
    fadeAmount = FADE;
    Serial.println("Welcome to the Restore the Light Game. Press Key B1 to Start");
    enableInterrupt(buttonLedArr[0].buttonPin, startGame, RISING);
    setTimerOne(SLEEP_TIME, sleep);
    state = starting;
}

void waitingStart() {
    int tmpPotVal = map(analogRead(POTENTIOMETER), POT_MIN, POT_MAX, MIN_DIFF, MAX_DIFF);
    if (potVal != tmpPotVal) {
        restartTimerOne(SLEEP_TIME, sleep);
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
    if (avoidButtonsBouncing()) {
        switchOff();
        Serial.println("Go!");
        score = 0;
        setConcurrentState(showingOrder);
    }
}

void timeOut() {
    setConcurrentState(madeMistake);
}

void restart() {
    Serial.print("Game Over. Final Score: ");
    Serial.println(score);
    disableButtonsInterrupt();
    switchOff();
    generateTimes();
    resetTimerOne();
    digitalWrite(REDLED, HIGH);
    delay(DELAY);
    digitalWrite(REDLED, LOW);
    delay(MISTAKE_TIME);
    Serial.flush();
    state = settingUp;
}

void startTurningOffLeds() {
    int* arr = setupTurningOffLeds();
    for (int i=0; i<COUPLES; i++) {
        delay(times[1]/4);
        arr[i] = randomLedOrder(arr);
        digitalWrite(buttonLedArr[arr[i]].ledPin, LOW);
        buttonLedArr[arr[i]].turn = i;
    }
    currentTurn = COUPLES-1;
    setTimerOne(times[2] * 1000000, timeOut);
    enableButtonsInterrupt(buttonPressed);
    state = waitingPlayer;
}

int* setupTurningOffLeds() {
    resetTimerOne();
    difficulty = 1 - (potVal/10);
    reduceTimes();
    switchGreens(true);
    int arr[COUPLES];
    for (int i = 0; i < COUPLES; i++) {
        arr[i] = UNDEFINED;
        buttonLedArr[i].turn = UNDEFINED;
    }
    delay(times[0]);
    return arr;
}

int randomLedOrder(int *arr) {
    int randomNumber = randomInt(0, COUPLES-1);
    while (isPresent(arr, randomNumber)) {
        randomNumber = randomInt(0, COUPLES-1);
    }
    return randomNumber;
}

void buttonPressed() {
    noInterrupts();
    if (avoidButtonsBouncing()) {
        for (int i = 0; i < COUPLES; i++) {
            if (digitalRead(buttonLedArr[i].buttonPin) == HIGH && buttonLedArr[i].turn != UNDEFINED) {
                if (buttonLedArr[i].turn == currentTurn) {
                    correctGuess(i);
                } else {
                    state = madeMistake;
                }
                break;
            }
        }
    }
    interrupts();
}

void correctGuess(int index) {
    currentTurn--;
    buttonLedArr[index].turn = UNDEFINED;
    digitalWrite(buttonLedArr[index].ledPin, HIGH);
    if (currentTurn < 0) {
        score += 1 + difficulty;
        Serial.print("New point! Score: ");
        Serial.println(score);
        state = showingOrder;
    }
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

void reduceTimes() {
    for (int i = 1; i < TIMERS; i++) {
        times[i] *= difficulty;
    };
}

void sleep() {
    Serial.println("GOING TO POWER DOWN IN 1 SECOND...");
    delay(DELAY);
    switchOff();
    state = sleeping;
    set_sleep_mode(SLEEP_MODE_PWR_DOWN);
    sleep_enable();
    enableButtonsInterrupt(wakeUp);
    sleep_mode();
    sleep_disable();
    disableButtonsInterrupt();
    Serial.flush();
}

void wakeUp() {
    prevts = millis();
    state = settingUp;
}