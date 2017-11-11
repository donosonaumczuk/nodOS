#include <videoDriver.h>

#define ZERO_EXCEPTION_ID 0
#define OVERFLOW_EXCEPTION_ID 1
#define INVALIDOPCODE_EXCEPTION_ID 2

static void zero_division();
static void over_flow();
static void invalid_operation_code();

void exceptionDispatcher(int exception) {
	if(exception == ZERO_EXCEPTION_ID)
		zero_division();
	if(exception == OVERFLOW_EXCEPTION_ID)
		over_flow();
	if(exception == INVALIDOPCODE_EXCEPTION_ID)
		invalid_operation_code();
}

static void zero_division() {
	clear();
	printWithColor("Error: Divide by zero\n",22,0x0F);
	haltCycle();
}

static void over_flow(){
	clear();
	printWithColor("Error: Overflow\n",16,0x0F);
	haltCycle();
}

static void invalid_operation_code(){
	clear();
	printWithColor("Error: Invalid opcode\n",23,0x0F);
	haltCycle();
}
