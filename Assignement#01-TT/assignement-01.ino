#include <EnableInterrupt.h>

const int size = 4;
const int redLed = 13;
const int greenLed1 = 12;
const int greenLed2 = 11;
const int greenLed3 = 10;
const int greenLed4 = 9;
const int button1 = 3;
const int button2 = 2;
const int button3 = 1;
const int button4 = 0;
const int potentiometer = A0;
const fade = 2;

bool start = false;

typedef struct {
    int buttonPin;
    int ledPin;
} buttonLed;

void setup() {
    buttonLed buttonLedArr[size] = {
            {button1, greenLed1},
            {button2, greenLed2},
            {button3, greenLed3},
            {button4, greenLed4}
    };
    for (int i = 0; i < size; i++) {
        pinMode(buttonLedArr[i].buttonPin, INPUT);
        enableInterrupt(buttonLedArr[i].buttonPin, buttonPressed, CHANGE);
        pinMode(buttonLedArr[i].ledPin, OUTPUT);
    }
    pinMode(redLed, OUTPUT);
    Serial.begin(115200);
}

void loop() {
    redFade(fade);
}

void redFade(int fadeAmount) {
    Serial.println("Welcome to the Restore the Light Game. Press Key B1 to Start");
    int brightness = 0;
    noInterrupts();
    while (!start) {
        interrupts();
        analogWrite(redLed, brightness);
        brightness = brightness + fadeAmount;
        if (brightness == 0 || brightness == 255) {
        fadeAmount = -fadeAmount ;
        }
        delay(30); 
    }
}


void buttonPressed() {
    start = digitalRead(button1) == HIGH ? true : false;
}

