#include "Led.h"
#include <Arduino.h>

Led::Led(int pin) {
    this->pin = pin;
    pinMode(pin, OUTPUT);
}