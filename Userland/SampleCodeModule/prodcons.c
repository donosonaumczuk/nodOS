#include <prodcons.h>
#include <mutualExclusion.h>
#include <semaphore.h>
#include <null.h>

void terminateAll(prodcons_t *prodcons);

void prodcons() {
	printMenu();
	createMutualExclusion(MUTEX_PRODCONS);
	createSemaphore(SEM_FULL, BUFFER_SIZE);
	createSemaphore(SEM_EMPTY, 0);
	char criticalZone[BUFFER_SIZE + 1];
	initCriticalZone(criticalZone);
	int index = 0;

	printCriticalZone(criticalZone);

	uint64_t consumers[MAX_PRODCONS] = {0};
	int consumersQty = 0;
	uint64_t producers[MAX_PRODCONS] = {0};
	int producersQty = 0;

	prodcons_t *consumerStruct = allocateMemory(sizeof(prodcons_t));
	consumerStruct->list = consumers;
	consumerStruct->size = &consumersQty;
	consumerStruct->criticalZone = criticalZone;
	consumerStruct->index = &index;

	prodcons_t *producerStruct = allocateMemory(sizeof(prodcons_t));
	producerStruct->list = producers;
	producerStruct->size = &producersQty;
	producerStruct->criticalZone = criticalZone;
	producerStruct->index = &index;

	int c;
	while((c = getchar()) != QUIT) {
		switch(c) {
			case INC_PROD:
				incrementProducer(producerStruct);
				break;
			case DEC_PROD:
				decrementProducer(producerStruct);
				break;
			case INC_CONS:
				incrementConsumer(consumerStruct);
				break;
			case DEC_CONS:
				decrementConsumer(consumerStruct);
				break;
			default:
				break;
		}
	}

	terminateAll(producerStruct);
	terminateAll(consumerStruct);
	terminateMutualExclusion(MUTEX_PRODCONS);
	terminateSemaphore(SEM_FULL);
	terminateSemaphore(SEM_EMPTY);
	printf("Program finished with quit command succesfully\n");
}

void terminateAll(prodcons_t *prodcons) {
	for(int i = 0; i < MAX_PRODCONS; i++) {
		if(prodcons->list[i] != 0) {
			terminateProcess(prodcons->list[i]);
		}
	}
}

void printCriticalZone(char *criticalZone) {
	changeFontColor(GREEN);
	printf("Critical zone buffer = ");
	printf("[ ");
	printf(criticalZone);
	printf(" ]\n\n\n");
	changeFontColor(WHITE);
}

void initCriticalZone(char *criticalZone) {
	for(int i = 0; i < BUFFER_SIZE; i++) {
		criticalZone[i] = EMPTY_SPACE;
	}
	criticalZone[BUFFER_SIZE] = 0;
}

void incrementProducer(prodcons_t *producerStruct) {
	lock(MUTEX_PRODCONS);
	int *size = producerStruct->size;
	if(*size >= MAX_PRODCONS) {
		changeFontColor(RED);
		printf("Produces already reach the max quantity\n");
		changeFontColor(WHITE);
		unlock(MUTEX_PRODCONS);
		return;
	}

	void ** arguments = allocateMemory(sizeof(void*) * 6);
	int mode = FOREGROUND;
	int *id = allocateMemory(sizeof(*id));
	*id = *size;
	arguments[0] = &mode;
	arguments[1] = "producer";
	arguments[2] = NULL;
	arguments[3] = NULL;
	arguments[4] = producerStruct;
	arguments[5] = id;
	producerStruct->list[*id] = createProcess(&producer, 4, arguments);
	(*size)++;
	unlock(MUTEX_PRODCONS);
}

void decrementProducer(prodcons_t *producerStruct) {
	lock(MUTEX_PRODCONS);
	int *size = producerStruct->size;
	if(*size == 0) {
		changeFontColor(RED);
		printf("Producers already reach the min quantity\n");
		changeFontColor(WHITE);
		unlock(MUTEX_PRODCONS);
		return;
	}

	(*size)--;
	terminateProcess(producerStruct->list[*size]);
	producerStruct->list[*size] = 0;

	changeFontColor(BLUE);
	printf("You killed an inocent producer :(\n");
	changeFontColor(WHITE);

	unlock(MUTEX_PRODCONS);
}

void incrementConsumer(prodcons_t *consumerStruct) {
	lock(MUTEX_PRODCONS);
	int *size = consumerStruct->size;
	if(*size >= MAX_PRODCONS) {
		changeFontColor(RED);
		printf("Consumers already reach the max quantity\n");
		changeFontColor(WHITE);
		unlock(MUTEX_PRODCONS);
		return;
	}

	void ** arguments = allocateMemory(sizeof(void*) * 6);;
	int mode = FOREGROUND;
	int *id = allocateMemory(sizeof(*id));
	*id = *size;
	arguments[0] = &mode;
	arguments[1] = "consumer";
	arguments[2] = NULL;
	arguments[3] = NULL;
	arguments[4] = consumerStruct;
	arguments[5] = id;
	consumerStruct->list[*id] = createProcess(&consumer, 4, arguments);
	(*size)++;
	unlock(MUTEX_PRODCONS);
}

void decrementConsumer(prodcons_t *consumerStruct) {
	lock(MUTEX_PRODCONS);
	int *size = consumerStruct->size;
	if(*size == 0) {
		changeFontColor(RED);
		printf("Consumers already reach the min quantity\n");
		changeFontColor(WHITE);
		unlock(MUTEX_PRODCONS);
		return;
	}

	(*size)--;
	terminateProcess(consumerStruct->list[*size]);

	changeFontColor(BLUE);
	printf("You killed an inocent consumer :()\n");
	changeFontColor(WHITE);
	consumerStruct->list[*size] = 0;

	unlock(MUTEX_PRODCONS);
}

int getNotNullId(prodcons_t *prodcons) {
	for(int i = 0; i < MAX_PRODCONS; i++) {
		if(prodcons->list[i] != 0) {
			return i;
		}
	}

	return -1; /* can't happen */
}

int getNullId(prodcons_t *prodcons) {
	for(int i = 0; i < MAX_PRODCONS; i++) {
		if(prodcons->list[i] == 0) {
			return i;
		}
	}

	return -1; /* can't happen */
}

int producer(int argc, void ** args) {
	prodcons_t *producerStruct = args[0];

	printf("Hello I'm a new producer. I want to write!\n");
	while(1) {
		semaphoreWait(SEM_FULL);
		lock(MUTEX_PRODCONS);

		/* Write on critical zone my id */
		int index = *producerStruct->index;
		*producerStruct->index = index + 1;

		producerStruct->criticalZone[index] = '0';

		printCriticalZone(producerStruct->criticalZone);

		semaphorePost(SEM_EMPTY);
		unlock(MUTEX_PRODCONS);;
	}
	return 0;
}

int consumer(int argc, void ** args) {
	prodcons_t *consumerStruct = args[0];

	printf("Hello I'm a new consumer. I want to consume!\n");
	while(1) {
		semaphoreWait(SEM_EMPTY);
		lock(MUTEX_PRODCONS);

		/* Read on critical zone writing it as empty */
		int index = *consumerStruct->index;
		*consumerStruct->index = index - 1;
		//int consumedInt = consumerStruct->criticalZone[index] - '0';
		consumerStruct->criticalZone[index] = EMPTY_SPACE;

		printCriticalZone(consumerStruct->criticalZone);

		semaphorePost(SEM_FULL);
		unlock(MUTEX_PRODCONS);
	}
}

void printMenu() {
	printf("You can create at least %d consumers and producer\n", MAX_PRODCONS);
	printf("Press:\n");
	putChar(INC_PROD);
	printf(": Increment producers\n");
	putChar(DEC_PROD);
	printf(": Decrement producers\n");
	putChar(INC_CONS);
	printf(": Increment consumers\n");
	putChar(DEC_CONS);
	printf(": Decrement consumers\n");
	putChar(QUIT);
	printf(": Quit\n\n\n");
}
