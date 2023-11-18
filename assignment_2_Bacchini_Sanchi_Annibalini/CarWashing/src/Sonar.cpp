#include "include/Sonar.h"
#include <Arduino.h>

Sonar::Sonar(int pinIN, int pinOUT, float velocitaSuono) {
    this->pinIn = pinIN;
    this->pinOut = pinOUT;
    this->velocitaSuono = velocitaSuono;
    Serial.begin(115200);
    pinMode(pinIn, INPUT);
    pinMode(pinOut, OUTPUT);
}

float Sonar::getDistance() {
    digitalWrite(pinOut, LOW);
    delayMicroseconds(5);
    digitalWrite(pinOut, HIGH);
    delayMicroseconds(5);
    digitalWrite(pinOut, LOW);
    float tUS = pulseIn(pinIn, HIGH);
    float t = tUS / 2000000.0;
    float d = t*velocitaSuono;
    return d;
}