#include <scheduler.h>

typedef struct scheduler_t {
   processControlBlockListPtr_t ready;
   processControlBlockListPtr_t waiting;
   processControlBlockListPtr_t terminated;
} scheduler_t;

void _force_context_switch(void);
int mutex_lock(uint8_t * arg);


static uint8_t ticksPassed = 0;
static scheduler_t scheduler;
static int isFirst = TRUE;

void initializeScheduler() {
    scheduler.ready = initializePCBList();
    scheduler.waiting = initializePCBList();
    scheduler.terminated = initializePCBList();
    createMutualExclusion(SCHEDULER_MUTEX_ID, SCHEDULER_PROCESS_ID);
    lockIfUnlocked(SCHEDULER_MUTEX_ID, SCHEDULER_PROCESS_ID);
}

void startScheduler() {
    unlock(SCHEDULER_MUTEX_ID, SCHEDULER_PROCESS_ID);
    _force_context_switch();
}

void * schedule(void * currentProcessStackPointer) {
	ticksPassed ++;
    void * aux = currentProcessStackPointer;

	if(ticksPassed == QUANTUM) {
        ticksPassed = 0;
    }
    if(ticksPassed == 0 && lockIfUnlocked(SCHEDULER_MUTEX_ID, SCHEDULER_PROCESS_ID))  {
        if(isFirst) {
            isFirst = FALSE;
        }
        else {
            nextProcess(currentProcessStackPointer);
        }
        aux = getStackPointer(getCurrentPCB());
        unlock(SCHEDULER_MUTEX_ID, SCHEDULER_PROCESS_ID);
    }

	return aux;
}

void nextProcess(void * currentProcessStackPointer) {
    processControlBlockPtr_t pcb = removeFirstPCBFromList(scheduler.ready);
    setStackPointer(pcb, currentProcessStackPointer);
    addProcessToScheduler(pcb);
}

void addProcessToScheduler(processControlBlockPtr_t pcb) {
    if(isReady(pcb)) {
        addPCBToList(scheduler.ready, pcb);
    }
	else if(isWaiting(pcb) || isBlocked(pcb)) {
        addPCBToList(scheduler.waiting, pcb);
    }
    else if(isTerminate(pcb)) {
        addPCBToList(scheduler.terminated, pcb);
    }
}

void terminateCurrentProcess(int returnValue) {
    terminateAProcess(returnValue, getCurrentPCB());
}

void terminateAProcess(int returnValue, processControlBlockPtr_t pcb) {
    processControlBlockPtr_t currentPCB = pcb;
    processControlBlockPtr_t currentPCBFather = getFather(currentPCB);
    giveChildsToFather(currentPCB);
    if(isWaiting(currentPCBFather)) {
        wakeUp(getProcessIdOf(getFather(currentPCB)));
    }
    setReturnValue(currentPCB, returnValue);
    freeStack(currentPCB);
    setState(currentPCB, PROCESS_TERMINATE);
    _force_context_switch();
}

void terminateAProcessByPid(uint8_t pid) {
    if(pid != 1 && pid != 2) {
        processControlBlockPtr_t pcb = PCBFromListByPID(scheduler.ready, pid);
        if(pcb == NULL) {
            pcb = PCBFromListByPID(scheduler.waiting, pid);
        }
        if(pcb != NULL) {
            terminateAProcess(-1, pcb);
        }
    }

}

void sleepCurrent(int condition) {
    processControlBlockPtr_t currentPCB = getCurrentPCB();
    setState(currentPCB, condition);
    _force_context_switch();
}

void wakeUp(uint64_t pid) {
    processControlBlockPtr_t pcb = PCBFromListByPID(scheduler.waiting, pid);
    if(pcb != NULL) {
        setState(pcb, PROCESS_READY);
        addProcessToScheduler(pcb);
    }
}

int waitChild(uint64_t pid) {
    processControlBlockPtr_t father = getCurrentPCB();
    processControlBlockPtr_t son = PCBFromListByPID(getSons(father), pid);
    if(son != NULL) {
        while (!isTerminate(son)) {
            sleepCurrent(PROCESS_WAITING);
        }
    }
    PCBFromListByPID(scheduler.terminated, pid);
    freeMemory(son);
    return getReturnValue(son);
}

processControlBlockPtr_t getASonOfCurrentProcess() {
    processControlBlockPtr_t current = getCurrentPCB();
    processControlBlockListPtr_t currentSons = getSons(current);
    return consultFirstPCBFromList(currentSons);
}

processControlBlockPtr_t getCurrentPCB() {
    return consultFirstPCBFromList(scheduler.ready);
}

uint64_t getProcessId() {
    return getProcessIdOf(consultFirstPCBFromList(scheduler.ready));
}

int isCurrentForeground() {
    return isForeground(getCurrentPCB());
}

void printAllProcess() {
    printWithColor("N A M E             |   I D   |     M O D E      |  S T A T E\n", 62, 0x0F);
    printWithColor("-------------------------------------------------------------\n", 62, 0x0F);
    printList(scheduler.ready);
    printList(scheduler.waiting);
    printList(scheduler.terminated);
}
