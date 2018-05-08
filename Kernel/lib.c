#include <stdint.h>

#define START 0x1000000

void * memset(void * destination, int32_t c, uint64_t length) {
	uint8_t chr = (uint8_t)c;
	char * dst = (char*)destination;

	while(length--)
		dst[length] = chr;

	return destination;
}

void * memcpy(void * destination, const void * source, uint64_t length) {
	/*
	* memcpy does not support overlapping buffers, so always do it
	* forwards. (Don't change this without adjusting memmove.)
	*
	* For speedy copying, optimize the common case where both pointers
	* and the length are word-aligned, and copy word-at-a-time instead
	* of byte-at-a-time. Otherwise, copy by bytes.
	*
	* The alignment logic below should be portable. We rely on
	* the compiler to be reasonably intelligent about optimizing
	* the divides and modulos out. Fortunately, it is.
	*/
	uint64_t i;

	if ((uint64_t)destination % sizeof(uint32_t) == 0 &&
		(uint64_t)source % sizeof(uint32_t) == 0 &&
		length % sizeof(uint32_t) == 0) {
		uint32_t *d = (uint32_t *) destination;
		const uint32_t *s = (const uint32_t *)source;

		for (i = 0; i < length / sizeof(uint32_t); i++)
			d[i] = s[i];
	}
	else {
		uint8_t * d = (uint8_t*)destination;
		const uint8_t * s = (const uint8_t*)source;

		for (i = 0; i < length; i++)
			d[i] = s[i];
	}

	return destination;
}


void stringCopy(char *destination,const char *source) {
	int i = 0;

	while(source[i] != 0) {
		destination[i] = source[i];
		i++;
	}

	destination[i] = 0;
}

int stringLength(const char *string) {
	int length = 0;

	while(string[length] != 0) {
		length++;
	}

	return length;
}

char *stringConcatenation(const char *s1,const char *s2) {
	unsigned int s1Length = stringLength(s1);
	unsigned int s2Length = stringLength(s2);
	char *result = allocateMemory(s1Length + s2Length + 1);
	stringCopy(result,s1);
	stringCopy(result + s1Length, s2);
	result[s1Length + s2Length] = 0;
	printWithColor(result, s1Length + s2Length + 1, 49);
	newLine();
	return result;
}
