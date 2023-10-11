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
int score;
float potentiometerValue;
int t1, t2, t3;
volatile long prevts;

void setup();
void loop();
void setState(gameState newState);
void waitingStart(int fadeAmount);
void buttonPressed();
void losingCase();
void startLeds();
void switchGreens(bool state);
int randomLedOrder(int *arr);
bool isPresent(int *arr, int num);
void generateTimes();
void setTimes();
int randomInt(int min, int max);
float randomFloat(float min, float max);
void wakeUp();

void setup() {
    Serial.begin(115200);
    state = starting;
    currentTurn = 0;
    score = 0;
    prevts = 0;
    // potentiometerValue = analogRead(POTENTIOMETER);
    potentiometerValue = 0.9;
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
    Timer1.initialize(15000000);
    Timer1.attachInterrupt(sleep);
}

void loop() {
    // potentiometerValue = analogRead(POTENTIOMETER);
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
            losingCase();
            setState(starting);
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
    int brightness = 0;
    gameState tmp = starting;
    while (tmp == starting) {
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
}

void buttonPressed() {
    noInterrupts();
    long ts = millis();
    if (ts - prevts > 40) {
        prevts = ts;
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
                            score++;
                            Serial.print("New point! Score: ");
                            Serial.println(score);
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

void losingCase() {
    score = 0;
    switchGreens(false);
    digitalWrite(REDLED, HIGH);
    delay(1000);
    digitalWrite(REDLED, LOW);
    delay(10000);
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

void sleep() {
    Serial.println("Mi Mi Mi");
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
    Serial.println("Waking up");
}

void wakeUp() {
    setState(starting);
}

int randomInt(int min, int max) {
    return (rand() % (max - min + 1)) + min;
}

float randomFloat(float min, float max) {
    return ((float)rand() / (float)RAND_MAX) * (max - min) + min;
}

void generateTimes() {
    t1 = (int)randomFloat(((float)1000)*potentiometerValue, ((float)3000)*potentiometerValue); // time to start the game
    t2 = (int)randomFloat(((float)3000)*potentiometerValue, ((float)5000)*potentiometerValue); // time to turn on the leds
    t3 = (int)randomFloat(((float)4000)*potentiometerValue, ((float)6000)*potentiometerValue); // time to wait for the player
}

void setTimes() {
    t1 *= potentiometerValue;
    t2 *= potentiometerValue;
    t3 *= potentiometerValue;
}