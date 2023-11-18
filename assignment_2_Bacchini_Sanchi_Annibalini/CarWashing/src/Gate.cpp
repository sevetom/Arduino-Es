#include "inlcude/Gate.h"
#include <Arduino.h>

//ATTENZIONE NON SONO SICURO CHE SIA GIUSTO
//FORSE USARE LIBRERIA "Servo.h"
Gate::Gate(int pin) {
    this->pin = pin;
    pinMode(pin, OUTPUT);
    //Di default chiuso
    digitalWrite(pin, LOW);
    state = false;
}

bool Gate::isOpen() {
    return state;
}

//ATTENZIONE NON SONO SICURO CHE SIA GIUSTO
void Gate::setOpen() {
    digitalWrite(pin, HIGH);
    state = true;
}

void Gate::setClose() {
    digitalWrite(pin, LOW);
    state = false;
}
