#include <Arduino.h>
#include <EnableInterrupt.h>
#include "Scheduler.h"
#include "TaskHandler.h"
#include "SleepHandler.h"

#define TASK_HANDLERS 7

typedef enum {
  SLEEPING,
  WELCOME,
  PROCEEDING,
  ENTERED,
  WASHING,
  EXITING,
  LEAVED
} taskHandlerType;

void changeTasks();
void insertTasks(Task** list);

Scheduler sched;
TaskHandler* taskHandlers[TASK_HANDLERS];
int currentHandler;

void setup() {
	Serial.begin(9600);
  taskHandlers[SLEEPING] = new SleepHandler();
  /*
  taskHandlers[WELCOME] = new WelcomeHandler();
  taskHandlers[PROCEEDING] = new ProceedingHandler();
  taskHandlers[ENTERED] = new EnteredHandler();
  taskHandlers[WASHING] = new WashingHandler();
  taskHandlers[EXITING] = new ExitingHandler();
  taskHandlers[LEAVED] = new LeavedHandler();
  */
  for (int i = 0; i < TASK_HANDLERS; i++){
    taskHandlers[i]->initTasks();
  }
  currentHandler = 0;
  enableInterrupt(taskHandlers[currentHandler]->getInterruptPin(), changeTasks, CHANGE);
  insertTasks(taskHandlers[currentHandler]->getTasks());
	sched.init(50);
}

void loop() {
	sched.schedule();
}

void changeTasks() {
  sched.removeTasks(taskHandlers[currentHandler]->getTasks());
  taskHandlers[currentHandler]->afterInterrupt();
	disableInterrupt(taskHandlers[currentHandler]->getInterruptPin());
  currentHandler = currentHandler >= TASK_HANDLERS ? 0 : currentHandler+1;
  enableInterrupt(taskHandlers[currentHandler]->getInterruptPin(), changeTasks, CHANGE);
  insertTasks(taskHandlers[currentHandler]->getTasks());
}

void insertTasks(Task** list) {
  int cur = 0;
  while(list[cur] != NULL) {
    sched.addTask(list[cur]);
    cur++;
  }
}