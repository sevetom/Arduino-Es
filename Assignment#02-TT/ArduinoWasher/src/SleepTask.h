#ifndef __SLEEPTASK_H__
#define __SLEEPTASK_H__

#include "Task.h"
#include <avr/sleep.h>

class SleepTask: public Task {
  public:
    SleepTask();
    void init(int period);  
    void tick();
    void wakeUp();
};

#endif