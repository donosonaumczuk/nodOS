#ifndef COMPARE_FUNCTIONS_H
#define COMPARE_FUNCTIONS_H

#include <stdint.h>
#include <memoryAllocator.h> // for Null

#define EQUALS 0

int stringCompare(char *string, char *otherString);
int uintCompare(uint64_t number, uint64_t otherNumber);

#endif
