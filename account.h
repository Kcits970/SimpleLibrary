#define ID_MIN_CHARACTERS 4
#define PASSWORD_MIN_CHARACTERS 8
#define ACCOUNT_MAX_CHARACTERS 16
#define ACCOUNT_FILENAME "account.dat"

#include "random.h"

typedef struct {
	char* id;
	int idLength;
	char* password;
	int passwordLength;
	char encryptionKey;
} ACCOUNT;

ACCOUNT* getBlankAccount() {
	ACCOUNT* account = malloc(sizeof(ACCOUNT));
	account->id = NULL;
	account->password = NULL;
	account->encryptionKey = 0;

	return account;
}

void xorID(ACCOUNT* account) {
	for (int i = 0; i < account->idLength; i++)
		account->id[i] ^= account->encryptionKey;
}

void xorPassword(ACCOUNT* account) {
	for (int i = 0; i < account->passwordLength; i++)
		account->password[i] ^= account->encryptionKey;
}

void xorAccount(ACCOUNT* account) {
	xorID(account);
	xorPassword(account);
}

void setID(ACCOUNT* account, const char* id) {
	account->idLength = strlen(id);
	account->id = getEmptyString(strlen(id) + 1);
	strcpy(account->id, id);
}

void setPassword(ACCOUNT* account, const char* password) {
	account->passwordLength = strlen(password);
	account->password = getEmptyString(strlen(password) + 1);
	strcpy(account->password, password);
}

void freeAccount(ACCOUNT* account) {
	if (account == NULL) return;

	free(account->id);
	free(account->password);
	free(account);
}

int isIDValid(const char* id) {
	if (!checkStringLength(id, ID_MIN_CHARACTERS, ACCOUNT_MAX_CHARACTERS)) {
		printf("The number of characters in '%s' is not within [%d, %d]\n", id, ID_MIN_CHARACTERS, ACCOUNT_MAX_CHARACTERS);
		return 0;
	}

	return 1;
}

int isPasswordValid(const char* password) {
	if (!checkStringLength(password, PASSWORD_MIN_CHARACTERS, ACCOUNT_MAX_CHARACTERS)) {
		printf("The number of characters in '%s' is not within [%d, %d]\n", password, PASSWORD_MIN_CHARACTERS, ACCOUNT_MAX_CHARACTERS);
		return 0;
	}

	return 1;
}

int isAccountValid(ACCOUNT* account) {
	return isIDValid(account->id) && isPasswordValid(account->password);
}

void writeID(FILE* fp, ACCOUNT* account) {
	fwrite(&account->idLength, sizeof(int), 1, fp);
	fwrite(account->id, sizeof(char), account->idLength + 1, fp);
}

void writePassword(FILE* fp, ACCOUNT* account) {
	fwrite(&account->passwordLength, sizeof(int), 1, fp);
	fwrite(account->password, sizeof(char), account->passwordLength + 1, fp);
}

void writeEncryptionKey(FILE* fp, ACCOUNT* account) {
	fwrite(&account->encryptionKey, sizeof(char), 1, fp);
}

int writeAccountToFile(ACCOUNT* account) {
	FILE* fp = fopen(ACCOUNT_FILENAME, "wb");

	if (fp == NULL) {
		printf("FAILED TO WRITE ACCOUNT TO '%s'\n", ACCOUNT_FILENAME);
		return 0;
	}
	
	xorAccount(account);
	writeID(fp, account);
	writePassword(fp, account);
	writeEncryptionKey(fp, account);
	fclose(fp);

	return 1;
}

int registerAccount() {
	ACCOUNT* account = getBlankAccount();

	printFormattedTitle("REGISTRATION PROMPT");

	getInputInDefaultBuffer("ID: ");
	setID(account, defaultBuffer);

	getInputInDefaultBuffer("PASSWORD: ");
	setPassword(account, defaultBuffer);

	account->encryptionKey = getRandomAlphabet();
	
	int isRegistrationSuccessful = isAccountValid(account) && writeAccountToFile(account);
	if (!isRegistrationSuccessful)
		printf("REGISTRATION FAILED\n");
	else
		printf("REGISTRATION SUCCESSFUL\n");

	freeAccount(account);
	return isRegistrationSuccessful;
}

void readID(FILE* fp, ACCOUNT* account) {
	fread(&account->idLength, sizeof(int), 1, fp);
	account->id = getEmptyString(account->idLength + 1);
	fread(account->id, sizeof(char), account->idLength + 1, fp);
}

void readPassword(FILE* fp, ACCOUNT* account) {
	fread(&account->passwordLength, sizeof(int), 1, fp);
	account->password = getEmptyString(account->passwordLength + 1);
	fread(account->password, sizeof(char), account->passwordLength + 1, fp);
}

void readEncryptionKey(FILE* fp, ACCOUNT* account) {
	fread(&account->encryptionKey, sizeof(char), 1, fp);
}

ACCOUNT* readAccountFromFile() {
	ACCOUNT* account = getBlankAccount();
	FILE* fp = fopen(ACCOUNT_FILENAME, "rb");

	if (fp == NULL)
		return NULL;

	readID(fp, account);
	readPassword(fp, account);
	readEncryptionKey(fp, account);
	xorAccount(account);
	return account;
}

int doesIDMatch(ACCOUNT* account, const char* id) {
	return strcmp(account->id, id) == 0;
}

int doesPasswordMatch(ACCOUNT* account, const char* password) {
	return strcmp(account->password, password) == 0;
}

int doesAccountMatch(ACCOUNT* account, const char* id, const char* password) {
	if (account == NULL) return 0;

	return doesIDMatch(account, id) && doesPasswordMatch(account, password);
}

int login() {
	printFormattedTitle("LOGIN PROMPT");
	char* id = getEmptyString(DEFAULT_BUFFER_LENGTH);
	char* password = getEmptyString(DEFAULT_BUFFER_LENGTH);

	getUserInput("ID: ", id, DEFAULT_BUFFER_LENGTH);
	getUserInput("PASSWORD: ", password, DEFAULT_BUFFER_LENGTH);

	ACCOUNT* account = readAccountFromFile();

	int isLoginSuccessful = doesAccountMatch(account, id, password);
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
	int exitAccountPrompt = 0;

	while (!exitAccountPrompt) {
		printAccountOptions();
		optionSelection = getCharacterInput(">>");

		switch (optionSelection) {
		case 'A':
			registerAccount();
			break;
		case 'B':
			if (login()) exitAccountPrompt = 1;
			break;
		default:
			printf("'%c' IS NOT AN IDENTIFIABLE COMMAND\n", optionSelection);
			break;
		}
	}
}