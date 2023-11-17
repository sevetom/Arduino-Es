#ifndef __SLEEP_HANDLER__
#define __SLEEP_HANDLER__

#include "TaskHandler.h"
#include "SleepTask.h"

#define PHOTORES_PIN A0

class SleepHandler : public TaskHandler {
public:
  void initTasks();
  int getInterruptPin();
  void afterInterrupt();
};

#endif