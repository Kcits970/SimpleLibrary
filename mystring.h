#ifndef MYSTRING_H
#define MYSTRING_H

char* getEmptyString(int characters) {
	return calloc(characters, sizeof(char));
}

int checkStringLength(const char* str, int lowerBound, int upperBound) {
	int length = strlen(str);
	if (length >= lowerBound && length <= upperBound)
		return 1;
	return 0;
}

int removeNewline(char* str) {
	char* lnptr = strchr(str, '\n');
	if (lnptr == NULL)
		return 0;
	else
		*lnptr = '\0';

	return 1;
}

#endif