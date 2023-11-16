#ifndef __TASK_HANDLER__
#define __TASK_HANDLER__

#include <EnableInterrupt.h>
#include "Task.h"

class TaskHandler {
public:
    virtual void init();
    Task** getTasks();
    virtual void toggleEndCondition(bool state, interruptFunctionType function);
};

#endif