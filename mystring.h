#ifndef MYSTRING_H_
#define MYSTRING_H_
#define CHAR_BS 8
#define CHAR_DEL 127

#include <string.h>

char* getEmptyString(int characters) {
	return calloc(characters, sizeof(char));
}

bool checkStringLength(const char* str, int lowerBound, int upperBound) {
	int length = strlen(str);
	if (length >= lowerBound && length <= upperBound)
		return true;
	return false;
}

bool isStringEqualTo(const char* str1, const char* str2) {
	if (strcmp(str1, str2) == 0)
		return true;
	else
		return false;
}

bool isCharContained(const char* str, char c) {
	if (strchr(str, c) == NULL)
		return false;
	else
		return true;
}

bool appendChar(char character, char* str, int limit) {
	int length = strlen(str);
	if (length == limit)
		return false;
	else {
		str[length] = character;
		str[length + 1] = '\0';
		return true;
	}
}

bool removeLastChar(char* str) {
	int length = strlen(str);

	if (length == 0)
		return false;
	else {
		str[length - 1] = '\0';
		return true;
	}
}

bool removeNewline(char* str) {
	char* lnptr = strchr(str, '\n');
	if (lnptr == NULL)
		return false;
	else
		*lnptr = '\0';

	return true;
}

#endif
