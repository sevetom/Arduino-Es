#ifndef __GATE__
#define __GATE__

#include <Arduino.h>
#include <Servo.h>

class Gate {
    public:
        Gate(int pin);
        void setOpen();
        void setClose();
        bool isOpen();
    protected:
        int pin;
        bool state;
        Servo servo;
};
#endif