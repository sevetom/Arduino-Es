#ifndef __SONAR__
#define __SONAR__
class Sonar {
    public:
        Sonar(int pin);
        float getDistance();
    protected:
        int pin; 
};
#endif