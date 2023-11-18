#ifndef __TASK_HANDLER__
#define __TASK_HANDLER__

#include "Task.h"

class TaskHandler {

protected:
  Task** tasksHandled;

public:
  virtual void initTasks() {
    tasksHandled = new Task*[0];
  }

  virtual Task** getTasks() {
    return tasksHandled;
  }

  virtual int getInterruptPin() {
    return 0;
  }

  virtual void afterInterrupt() = 0;
};

#endif