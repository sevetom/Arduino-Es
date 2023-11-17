#include <Arduino.h>
#include "Scheduler.h"
#include "TaskHandler.h"

#define TASK_HANDLERS 7

void changeTasks();

Scheduler sched;
TaskHandler* taskHandlers[TASK_HANDLERS];
int currentHandler;

void setup() {
	Serial.begin(9600);
  for (int i = 0; i < TASK_HANDLERS; i++){
    taskHandlers[i]->init();
  }
  currentHandler = 0;
  taskHandlers[currentHandler]->toggleEndCondition(true, changeTasks);
  insertTasks(taskHandlers[currentHandler]->getTasks());
	sched.init(50);
}

void loop() {
	sched.schedule();
}

void changeTasks() {
  sched.emptyTasks();
	taskHandlers[currentHandler]->toggleEndCondition(false, NULL);
  currentHandler = currentHandler >= TASK_HANDLERS ? 0 : currentHandler++;
  taskHandlers[currentHandler]->toggleEndCondition(true, changeTasks);
  insertTasks(taskHandlers[currentHandler]->getTasks());
}

void insertTasks(Task** list) {
  int cur = 0;
  while(list[cur] != NULL) {
    sched.addTask(list[cur]);
    cur++;
  }
}