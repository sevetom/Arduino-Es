/**
 * @author tommaso.severi2@studio.unibo.it
 * @author tommaso.ceredi@studio.unibo.it
 * @brief Restore the lights minigame
 * @version 1.0
 */

#include "restoreTheLights.h"

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