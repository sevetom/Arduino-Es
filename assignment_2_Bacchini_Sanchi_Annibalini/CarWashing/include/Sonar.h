#ifndef __SONAR__
#define __SONAR__
class Sonar {
    public:
        Sonar(int pinIN, int pinOUT, float velocitaSuono);
        float getDistance();
    protected:
        int pinIn;
        int pinOut;
        float velocitaSuono; 
};
#endif