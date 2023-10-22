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
    timer = timer_create_default();
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
    Serial.begin(9600);
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
            timer.tick();
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

bool avoidButtonsBouncing() {
    long ts = millis();
    if (ts - prevts > BUTTONS_BOUNCING) {
        prevts = ts;
        return true;
    }
    return false;
}

void setupStart() {
    potVal = map(analogRead(POTENTIOMETER), POT_MIN, POT_MAX, MIN_DIFF, MAX_DIFF);
    brightness = 0;
    fadeAmount = FADE;
    Serial.println("Welcome to the Restore the Light Game. Press Key B1 to Start");
    enableInterrupt(buttonLedArr[0].buttonPin, startGame, RISING);
    timer.in(SLEEP_TIME, powerDown);
    state = starting;
}

void waitingStart() {
    timer.tick();
    float tmp = analogRead(POTENTIOMETER);
    float tmpPotVal = map(tmp, POT_MIN, POT_MAX, MIN_DIFF, MAX_DIFF);
    if (potVal != tmpPotVal) {
        potVal = tmpPotVal;
        timer.cancel();
        timer.in(SLEEP_TIME, powerDown);
    }
    analogWrite(REDLED, brightness);
    brightness = brightness + fadeAmount;
    if (brightness == 0 || brightness == 255) {
        fadeAmount = -fadeAmount;
    }
    delay(FADE_PERIOD);
}

void startGame() {
    if (avoidButtonsBouncing()) {
        switchOff();
        disableInterrupt(buttonLedArr[0].buttonPin);
        Serial.println("Go!");
        score = 0;
        setConcurrentState(showingOrder);
    }
}

bool timeOutGuess(void* arg) {
    setConcurrentState(madeMistake);
    return false;
}

void restart() {
    Serial.print("Game Over. Final Score: ");
    Serial.println(score);
    disableButtonsInterrupt();
    switchOff();
    timer.cancel();
    generateTimes();
    digitalWrite(REDLED, HIGH);
    delay(DELAY);
    digitalWrite(REDLED, LOW);
    delay(MISTAKE_TIME);
    Serial.flush();
    state = settingUp;
}

void startTurningOffLeds() {
    int arr[COUPLES];
    setupTurningOffLeds(arr);
    for (int i=0; i<COUPLES; i++) {
        delay(times[1]/4);
        arr[i] = randomLedOrder(arr);
        digitalWrite(buttonLedArr[arr[i]].ledPin, LOW);
        buttonLedArr[arr[i]].turn = i;
    }
    currentTurn = COUPLES-1;
    timer.in(times[2], timeOutGuess);
    enableButtonsInterrupt(buttonPressed);
    state = waitingPlayer;
}

void setupTurningOffLeds(int* arr) {
    timer.cancel();
    difficulty = 1 - (potVal/10);
    reduceTimes();
    switchGreens(true);
    for (int i = 0; i < COUPLES; i++) {
        arr[i] = UNDEFINED;
        buttonLedArr[i].turn = UNDEFINED;
    }
    delay(times[0]);
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
    long timeInc = 0;
    for (int i = 0; i < TIMERS; i++) {
        times[i] = (int)randomFloat((float)MIN_TIME + timeInc, (float)MAX_TIME + timeInc);
        timeInc += TIME_INCREASE;
    }
}

void reduceTimes() {
    for (int i = 1; i < TIMERS; i++) {
        times[i] *= difficulty;
    }
}

bool powerDown(void* arg) {
    delay(DELAY);
    switchOff();
    setConcurrentState(sleeping);
    set_sleep_mode(SLEEP_MODE_PWR_DOWN);
    sleep_enable();
    enableButtonsInterrupt(wakeUp);
    sleep_mode();
    sleep_disable();
    disableButtonsInterrupt();
    Serial.flush();
    return false;
}

void wakeUp() {
    if (avoidButtonsBouncing()) {
        state = settingUp;
    }
}