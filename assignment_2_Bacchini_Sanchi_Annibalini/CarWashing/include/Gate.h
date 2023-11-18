#ifndef __GATE__
#define __GATE__
class Gate {
    public:
        Gate(int pin);
        void setOpen();
        void setClose();
        bool isOpen();
    protected:
        int pin;
        bool state;
};
#endif