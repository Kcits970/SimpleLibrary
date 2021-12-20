#ifndef USERINPUT_H_
#define USERINPUT_H_
#define DEFAULT_BUFFER_LENGTH 1024

#include <conio.h>
#include "mystring.h"

char defaultBuffer[DEFAULT_BUFFER_LENGTH];

void getAsciiInput(const char* promptMessage, char* str, int inputLimit, const char* exclusions, bool censor) {
	if (exclusions == NULL) exclusions = "";
	printf(promptMessage);

	char c = 0;
	int charCount = 0;
	while (c = _getch(), c != '\r' && c != '\n') {
		if (c == CHAR_BS || c == CHAR_DEL) {
			if (charCount != 0) {
				printf("\b \b");
				removeLastChar(str);
				charCount--;
			}
			continue;
		}

		if (!isCharContained(exclusions, c)) {
			censor ? printf("*") : printf("%c", c);
			if (appendChar(c, str, inputLimit)) charCount++;
		}
	}

	_putch('\n');
}

void getInputInDefaultBuffer(const char* promptMessage, const char* exclusions, bool censor) {
	defaultBuffer[0] = '\0';
	getAsciiInput(promptMessage, defaultBuffer, DEFAULT_BUFFER_LENGTH, exclusions, censor);
}

char getCharacterInput(const char* promptMessage) {
	getInputInDefaultBuffer(promptMessage, NULL, false);
	return defaultBuffer[0];
}

int getNumberInput(const char* promptMessage, int lowerBound, int upperBound) {
	getInputInDefaultBuffer(promptMessage, NULL, false);

	int number = atoi(defaultBuffer);
	if (number < lowerBound || number > upperBound)
		return lowerBound;
	else
		return number;
}

#endif
