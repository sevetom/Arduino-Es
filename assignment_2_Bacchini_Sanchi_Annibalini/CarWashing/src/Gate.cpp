#include "include/Gate.h"

Gate::Gate(int pin) {
    this->pin = pin;
    this->state = false;
    servo.attach(pin);
}

void Gate::setOpen() {
    servo.write(180);
    this->state = true;
}

void Gate::setClose() {
    servo.write(0);
    this->state = false;
}

bool Gate::isOpen() {
    return this->state;
}