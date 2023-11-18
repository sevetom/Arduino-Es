#ifndef __SLEEP_TASK__
#define __SLEEP_TASK__

#include "Task.h"
#include <avr/sleep.h>

class SleepTask: public Task {
  public:
    SleepTask();
    void init(int period);  
    void tick();
};

#endif