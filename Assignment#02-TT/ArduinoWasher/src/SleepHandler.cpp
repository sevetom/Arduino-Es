#include "SleepHandler.h"

SleepTask* sleep;

void SleepHandler::initTasks(){
  pinMode(PHOTORES_PIN, INPUT);
  sleep = new SleepTask();
  tasksHandled[0] = sleep;
}

int SleepHandler::getInterruptPin() {
  return PHOTORES_PIN;
}

void SleepHandler::afterInterrupt() {
  sleep_disable();
}