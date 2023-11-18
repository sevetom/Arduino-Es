#include "inlcude/Gate.h"
#include <Arduino.h>

Gate::Gate(int pin) {
    this->pin = pin;
    pinMode(pin, OUTPUT);
    //Di default chiuso
    digitalWrite(pin, LOW);
    state = false;
}

void Gate::setOpen() {
    digitalWrite(pin, HIGH);
    state = true;
}

void Gate::setClose() {
    digitalWrite(pin, LOW);
    state = false;
}

bool Gate::isOpen() {
    return state;
}