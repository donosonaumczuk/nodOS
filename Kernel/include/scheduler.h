#ifndef _SCHEDULER_H_

#define _SCHEDULER_H_

#include <processControlBlockLib.h>
#include <stdint.h>
#include <memoryAllocator.h>
#include <videoDriver.h>
#include <mutualExclusion.h>

#define QUANTUM 1//quantity of ticks

void initializeScheduler();
void * schedule(void * currentProcessStackPointer);
void addProcessToScheduler(processControlBlockPtr_t pcb);
void terminateCurrentProcess();
void nextProcess(void * currentProcessStackPointer);
void startScheduler();
void sleepCurrent();
void wakeUp(uint64_t pid);
void waitChild(uint64_t pid);
processControlBlockPtr_t getASonOfCurrentProcess();
uint64_t getProcessID();

#endif
