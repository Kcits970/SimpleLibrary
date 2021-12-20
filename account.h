#ifndef ACCOUNT_H_
#define ACCOUNT_H_

#define ID_MIN_CHARACTERS 4
#define PASSWORD_MIN_CHARACTERS 8
#define ACCOUNT_MAX_CHARACTERS 16
#define ASCII_WHITESPACE " \t\n\r\v\f"
#define ACCOUNT_FILENAME "account.dat"

#include "mystring.h"

typedef struct {
	char* id;
	char* password;
} ACCOUNT;

ACCOUNT* getBlankAccount() {
	ACCOUNT* account = malloc(sizeof(ACCOUNT));
	account->id = NULL;
	account->password = NULL;

	return account;
}

void setID(ACCOUNT* account, const char* id) {
	account->id = getEmptyString(strlen(id) + 1);
	strcpy(account->id, id);
}

void setPassword(ACCOUNT* account, const char* password) {
	account->password = getEmptyString(strlen(password) + 1);
	strcpy(account->password, password);
}

void freeAccount(ACCOUNT* account) {
	if (account == NULL) return;

	free(account->id);
	free(account->password);
	free(account);
}

bool isIDValid(const char* id) {
	if (!checkStringLength(id, ID_MIN_CHARACTERS, ACCOUNT_MAX_CHARACTERS)) {
		printf("NUMBER OF ID CHARACTERS ARE NOT WITHIN [%d, %d]\n", id, ID_MIN_CHARACTERS, ACCOUNT_MAX_CHARACTERS);
		return false;
	}

	return true;
}

bool isPasswordValid(const char* password) {
	if (!checkStringLength(password, PASSWORD_MIN_CHARACTERS, ACCOUNT_MAX_CHARACTERS)) {
		printf("NUMBER OF PASSWORD CHARACTERS ARE NOT WITHIN [%d, %d]\n", password, PASSWORD_MIN_CHARACTERS, ACCOUNT_MAX_CHARACTERS);
		return false;
	}

	return true;
}

bool isAccountValid(ACCOUNT* account) {
	return isIDValid(account->id) && isPasswordValid(account->password);
}

bool writeAccountToFile(ACCOUNT* account, const char* filename) {
	FILE* fp = fopen(filename, "wb");
	if (fp == NULL) {
		printf("FAILED TO WRITE ACCOUNT TO '%s'\n", filename);
		return false;
	}
	
	writeString(fp, account->id);
	writeString(fp, account->password);
	fflush(fp);
	fclose(fp);

	return true;
}

bool registerAccount() {
	ACCOUNT* account = getBlankAccount();

	printFormattedTitle("REGISTRATION PROMPT");

	getInputInDefaultBuffer("ID: ", ASCII_WHITESPACE, false);
	setID(account, defaultBuffer);

	getInputInDefaultBuffer("PASSWORD: ", ASCII_WHITESPACE, true);
	setPassword(account, defaultBuffer);
	
	bool isRegistrationSuccessful = isAccountValid(account) && writeAccountToFile(account, ACCOUNT_FILENAME);
	if (!isRegistrationSuccessful)
		printf("REGISTRATION FAILED\n");
	else {
		printf("REGISTRATION SUCCESSFUL\n");
	}

	freeAccount(account);
}

ACCOUNT* readAccountFromFile(const char* filename) {
	FILE* fp = fopen(filename, "rb");
	if (fp == NULL)
		return NULL;

	ACCOUNT* account = getBlankAccount();

	bool isIDRead = readCharArray(fp, defaultBuffer, DEFAULT_BUFFER_LENGTH);
	setID(account, defaultBuffer);

	bool isPasswordRead = readCharArray(fp, defaultBuffer, DEFAULT_BUFFER_LENGTH);
	setPassword(account, defaultBuffer);

	if (!isIDRead || !isPasswordRead) {
		freeAccount(account);
	}

	return account;
}

bool doesAccountMatch(ACCOUNT* account, const char* id, const char* password) {
	if (account == NULL)
		return false;
	else
		return isStringEqualTo(account->id, id) && isStringEqualTo(account->password, password);
}

bool login() {
	printFormattedTitle("LOGIN PROMPT");
	char* id = getEmptyString(DEFAULT_BUFFER_LENGTH);
	char* password = getEmptyString(DEFAULT_BUFFER_LENGTH);

	getAsciiInput("ID: ", id, DEFAULT_BUFFER_LENGTH, ASCII_WHITESPACE, false);
	getAsciiInput("PASSWORD: ", password, DEFAULT_BUFFER_LENGTH, ASCII_WHITESPACE, true);

	ACCOUNT* account = readAccountFromFile(ACCOUNT_FILENAME);

	bool isLoginSuccessful = doesAccountMatch(account, id, password);
	if (!isLoginSuccessful)
		printf("LOGIN FAILED\n");
	else
		printf("LOGIN SUCCESSFUL\n");

	free(id);
	free(password);
	freeAccount(account);
	return isLoginSuccessful;
}

void printAccountOptions() {
	printFormattedTitle("ACCOUNT SETUP");
	printFormattedOption("REGISTER", 'A');
	printFormattedOption("LOGIN", 'B');
}

void promptAccountMenu() {
	char optionSelection = 0;
	bool exitAccountPrompt = false;

	while (!exitAccountPrompt) {
		printAccountOptions();
		optionSelection = getCharacterInput(">>");

		switch (optionSelection) {
		case 'A':
			registerAccount();
			break;
		case 'B':
			if (login()) exitAccountPrompt = true;
			break;
		default:
			printf("'%c' IS NOT AN IDENTIFIABLE COMMAND\n", optionSelection);
			break;
		}
	}
}

#endif