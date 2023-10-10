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

void loop();
void redFade(int fadeAmount);
void buttonPressed();
void losingCase();
void startLeds();
void switchGreens(bool state);
int randomLedOrder(int *arr);
bool isPresent(int *arr, int num);
void setT();

void setup() {
    Serial.begin(115200);
    state = starting;
    currentTurn = 0;
    score = 0;
    potentiometerValue = 1.0;
    setT();
    srand(time(NULL));
    buttonLedArr[0] = { BUTTON1, GREENLED1, UNDEFINED };
    buttonLedArr[1] = { BUTTON2, GREENLED2, UNDEFINED };
    buttonLedArr[2] = { BUTTON3, GREENLED3, UNDEFINED };
    buttonLedArr[3] = { BUTTON4, GREENLED4, UNDEFINED };
    for (int i = 0; i < SIZE; i++) {
        pinMode(buttonLedArr[i].buttonPin, INPUT);
        enableInterrupt(buttonLedArr[i].buttonPin, buttonPressed, CHANGE);
        pinMode(buttonLedArr[i].ledPin, OUTPUT);
    }
    pinMode(REDLED, OUTPUT);
    //Timer1.initialize(15000000);
    //Timer1.attachInterrupt(sleep);
}

void loop() {
    switch (state) {
        case starting:
            redFade(FADE);
            break;
        case waitingLed:
            startLeds();
            break;
        case waitingPlayer:
            break;
        case lost:
            Serial.println("You lost");
            losingCase();
            state = starting;
            break;
        case sleeping:
            break;
    }
}

void redFade(int fadeAmount) {
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
    if (state == starting && digitalRead(BUTTON1) == HIGH) {
        state = waitingLed;
    }
    if (state == waitingPlayer) {
        for (int i = 0; i < SIZE; i++) {
            noInterrupts();
            if (digitalRead(buttonLedArr[i].buttonPin) == HIGH && buttonLedArr[i].turn != UNDEFINED) {
                if (buttonLedArr[i].turn == currentTurn) {
                    currentTurn--;
                    buttonLedArr[i].turn = UNDEFINED;
                    digitalWrite(buttonLedArr[i].ledPin, HIGH);
                    if (currentTurn < 0) {
                        score++;
                        Serial.print("New point! Score: ");
                        Serial.println(score);
                        potentiometerValue = potentiometerValue * 0.9;
                        setT();
                        state = waitingLed;
                    }
                } else {
                    state = lost;
                }
            }
            interrupts();
            break;
        }
    }
    delay(500);
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
    state = waitingPlayer;
    for (int i = 0; i<SIZE; i++){
        Serial.print(arr[i]);
        Serial.print(" ");
    }
    Serial.println("");
}

void switchGreens(bool state) {
    for (int i = 0; i < SIZE; i++) {
        digitalWrite(buttonLedArr[i].ledPin, state ? HIGH : LOW);
    }
}

int randomLedOrder(int *arr) {
    int randomNumber = randomInt(0, 4);
    while (isPresent(arr, randomNumber)) {
        randomNumber = randomInt(0, 4);
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
    set_sleep_mode(SLEEP_MODE_PWR_DOWN);
    sleep_enable();
    for (int i = 0;i < SIZE; i++) {
        disableInterrupt(buttonLedArr[i].buttonPin);
        pinMode(buttonLedArr[i].buttonPin, INPUT);
        enableInterrupt(buttonLedArr[i].buttonPin, wakeUp, CHANGE);
    }
    sleep_mode();
    sleep_disable();
    for (int i = 0;i < SIZE; i++) {
        disableInterrupt(buttonLedArr[i].buttonPin);
        pinMode(buttonLedArr[i].buttonPin, INPUT);
        enableInterrupt(buttonLedArr[i].buttonPin, buttonPressed, CHANGE);
    }
    Serial.println("Waking up");
}

void wakeUp() {
    state = starting;
}

int randomInt(float min, float max) {
    return (int)(min + (max - min) * (float)rand() / (float)RAND_MAX);
}

void setT() {
    t1 = (int)randomInt((float)1000*potentiometerValue, (float)3000*potentiometerValue); // time to start the game
    t2 = (int)randomInt((float)3000*potentiometerValue, (float)5000*potentiometerValue); // time to turn on the leds
    t3 = (int)randomInt((float)4000*potentiometerValue, (float)6000*potentiometerValue); // time to wait for the player
    Serial.print("t1: ");
    Serial.print(t1);
    Serial.print(" | t2: ");
    Serial.print(t2);
    Serial.print(" | t3: ");
    Serial.println(t3);
}