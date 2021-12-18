#ifndef USERINPUT_H
#define USERINPUT_H
#define DEFAULT_BUFFER_LENGTH 1024

#include "mystring.h"

char defaultBuffer[DEFAULT_BUFFER_LENGTH];

void getUserInput(const char* promptMessage, char* str, int inputLimit) {
	printf(promptMessage);
	fgets(str, inputLimit, stdin);

	if (!removeNewline(str))
		while (getchar() != '\n');
}

void getInputInDefaultBuffer(const char* promptMessage) {
	getUserInput(promptMessage, defaultBuffer, DEFAULT_BUFFER_LENGTH);
}

char getCharacterInput(const char* promptMessage) {
	getInputInDefaultBuffer(promptMessage);
	return defaultBuffer[0];
}

int getNumberInput(const char* promptMessage, int lowerBound, int upperBound) {
	getInputInDefaultBuffer(promptMessage);

	int number = atoi(defaultBuffer);
	if (number < lowerBound || number > upperBound)
		return lowerBound;
	else
		return number;
}

#endif