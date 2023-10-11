#include <EnableInterrupt.h>
#include <Arduino.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <TimerOne.h>
#include <avr/sleep.h>

#define SIZE 4
#define REDLED 6
#define GREENLED1 12
#define GREENLED2 11
#define GREENLED3 10
#define GREENLED4 9
#define BUTTON1 5
#define BUTTON2 4
#define BUTTON3 3
#define BUTTON4 2
#define POTENTIOMETER A0
#define FADE 5
#define UNDEFINED -1
#define DELAY 1000
#define SLEEP_TIMER 10 * 1000000
// This are not standard potentiometer values but 
// the one we are using is really poor quality
#define POT_MIN 23
#define POT_MAX 1001

typedef struct {
    int buttonPin;
    int ledPin;
    int turn;
} buttonLed;

typedef enum {
    starting,
    waitingLed,
    waitingPlayer,
    lost,
    sleeping
} gameState;

volatile gameState state;
buttonLed buttonLedArr[SIZE];
volatile int currentTurn;
float score;
volatile float difficulty;
int t1, t2, t3;
volatile long prevts;

void setup();
void loop();
void setState(gameState newState);
void waitingStart(int fadeAmount);
void buttonPressed();
void startGame();
void timeOut();
void restart();
void startLeds();
void switchGreens(bool state);
int randomLedOrder(int *arr);
bool isPresent(int *arr, int num);
void generateTimes();
void setTimes();
int randomInt(int min, int max);
float randomFloat(float min, float max);
void switchOff();
void sleep();
void wakeUp();

void setup() {
    Serial.begin(115200);
    state = starting;
    currentTurn = 0;
    score = 0;
    prevts = 0;
    difficulty = 1;
    generateTimes();
    srand(analogRead(0));
    buttonLedArr[0] = { BUTTON1, GREENLED1, UNDEFINED };
    buttonLedArr[1] = { BUTTON2, GREENLED2, UNDEFINED };
    buttonLedArr[2] = { BUTTON3, GREENLED3, UNDEFINED };
    buttonLedArr[3] = { BUTTON4, GREENLED4, UNDEFINED };
    for (int i = 0; i < SIZE; i++) {
        pinMode(buttonLedArr[i].buttonPin, INPUT);
        enableInterrupt(buttonLedArr[i].buttonPin, buttonPressed, RISING);
        pinMode(buttonLedArr[i].ledPin, OUTPUT);
    }
    pinMode(REDLED, OUTPUT);
    Timer1.initialize(1000000);
    Timer1.stop();
}

void loop() {
    switch (state) {
        case starting:
            waitingStart(FADE);
            break;
        case waitingLed:
            startLeds();
            break;
        case waitingPlayer:
            break;
        case lost:
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

void waitingStart(int fadeAmount) {
    Serial.println("Welcome to the Restore the Light Game. Press Key B1 to Start");
    Timer1.setPeriod(SLEEP_TIMER);
    Timer1.attachInterrupt(sleep);
    int brightness = 0;
    float tmpPotentiometerValue = 0;
    gameState tmp = starting;
    while (tmp == starting) {
        tmpPotentiometerValue = map(analogRead(POTENTIOMETER), POT_MIN, POT_MAX, 1, 4);
        analogWrite(REDLED, brightness);
        brightness = brightness + fadeAmount;
        if (brightness == 0 || brightness == 255) {
            fadeAmount = -fadeAmount ;
        }
        noInterrupts();
        tmp = state;
        interrupts();
        delay(30);
    }
    analogWrite(REDLED, 0);
    Timer1.stop();
    Timer1.detachInterrupt();
    noInterrupts();
    difficulty = 1 - (tmpPotentiometerValue/10);
    setTimes();
    interrupts();
    Serial.print("Difficolta': ");
    Serial.println(difficulty);

}

bool avoidBounces() {
    long ts = millis();
    if (ts - prevts > 40) {
        prevts = ts;
        return true;
    }
    return false;
}

void buttonPressed() {
    noInterrupts();
    if (avoidBounces()) {
        if (state == starting && digitalRead(BUTTON1) == HIGH) {
            state = waitingLed;
        }
        if (state == waitingPlayer) {
            for (int i = 0; i < SIZE; i++) {
                if (digitalRead(buttonLedArr[i].buttonPin) == HIGH && buttonLedArr[i].turn != UNDEFINED) {
                    if (buttonLedArr[i].turn == currentTurn) {
                        currentTurn--;
                        buttonLedArr[i].turn = UNDEFINED;
                        digitalWrite(buttonLedArr[i].ledPin, HIGH);
                        if (currentTurn < 0) {
                            score+=difficulty+1;
                            Serial.print("New point! Score: ");
                            Serial.println(score);
                            Timer1.stop();
                            Timer1.detachInterrupt();
                            setTimes();
                            state = waitingLed;
                        }
                    } else {
                        state = lost;
                    }
                    break;
                }
            }
        }
        interrupts();
    }
}

void startGame() {
    Timer1.setPeriod(t3 * 1000000);
    Timer1.attachInterrupt(timeOut);
}

void timeOut() {
    setState(lost);
}

void restart() {
    score = 0;
    switchGreens(false);
    generateTimes();
    Timer1.stop();
    Timer1.detachInterrupt();
    digitalWrite(REDLED, HIGH);
    delay(1000);
    digitalWrite(REDLED, LOW);
    delay(10000);
    setState(starting);
}

void startLeds() {
    switchGreens(true);
    delay(t1);
    int arr[SIZE];
    for (int i = 0; i < SIZE; i++) {
        arr[i] = UNDEFINED;
        buttonLedArr[i].turn = UNDEFINED;
    }
    for (int i=0; i<SIZE; i++) {
        delay(t2/4);
        arr[i] = randomLedOrder(arr);
        digitalWrite(buttonLedArr[arr[i]].ledPin, LOW);
        buttonLedArr[arr[i]].turn = i;
    }
    currentTurn = SIZE-1;
    setState(waitingPlayer);
    startGame();
}

void switchGreens(bool state) {
    for (int i = 0; i < SIZE; i++) {
        digitalWrite(buttonLedArr[i].ledPin, state ? HIGH : LOW);
    }
}

int randomLedOrder(int *arr) {
    int randomNumber = randomInt(0, SIZE-1);
    while (isPresent(arr, randomNumber)) {
        randomNumber = randomInt(0, SIZE-1);
    }
    return randomNumber;
}

bool isPresent(int *arr, int num) {
    for (int i = 0; i < SIZE; i++) {
        if (arr[i] == num) {
            return true;
        }
    }
    return false;
}

void switchOff() {
    switchGreens(false);
    digitalWrite(REDLED, LOW);
    Serial.flush();
}

void sleep() {
    Serial.println("GOING TO POWER DOWN IN 1 SECOND...");
    delay(1000);
    switchOff();
    setState(sleeping);
    set_sleep_mode(SLEEP_MODE_PWR_DOWN);
    sleep_enable();
    for (int i = 0;i < SIZE; i++) {
        enableInterrupt(buttonLedArr[i].buttonPin, wakeUp, RISING);
    }
    sleep_mode();
    sleep_disable();
    for (int i = 0;i < SIZE; i++) {
        enableInterrupt(buttonLedArr[i].buttonPin, buttonPressed, RISING);
    }
}

void wakeUp() {
    if (avoidBounces()) {
        Serial.println("WAKING UP...");
        setState(starting);
    }
}

int randomInt(int min, int max) {
    return (rand() % (max - min + 1)) + min;
}

float randomFloat(float min, float max) {
    return ((float)rand() / (float)RAND_MAX) * (max - min) + min;
}

void generateTimes() {
    t1 = (int)randomFloat(((float)1000), ((float)3000)); // time to start the game
    t2 = (int)randomFloat(((float)4000), ((float)6000)); // time to turn on the leds
    t3 = (int)randomFloat(((float)5000), ((float)7000)); // time to wait for the player
}

void setTimes() {
    t1 *= difficulty;
    t2 *= difficulty;
    t3 *= difficulty;
    Serial.print("t1: ");
    Serial.print(t1);
    Serial.print(" | t2: ");
    Serial.print(t2);
    Serial.print(" | t3: ");
    Serial.println(t3);
}