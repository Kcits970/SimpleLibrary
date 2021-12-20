#define _CRT_SECURE_NO_WARNINGS
#define VERSION_ID 2
#define LIBRARY_CAPACITY 16

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

typedef enum {false=0, true=1} bool;

#include "userinput.h"
#include "formatting.h"
#include "binaryio.h"
#include "account.h"
#include "book.h"

BOOKSHELF* publicBookshelf = NULL;
BOOKSHELF* myBookshelf = NULL;
#define PUBLIC_BOOKSHELF_FILENAME "publicitems.dat"
#define MY_BOOKSHELF_FILENAME "myitems.dat"

void printMainOptions() {
	printFormattedTitle("MAIN OPTIONS");
	printFormattedOption("VIEW AVAILABLE ITEMS", 'A');
	printFormattedOption("ADD", 'B');
	printFormattedOption("DELETE", 'C');
	printFormattedOption("VIEW MY ITEMS", 'D');
	printFormattedOption("CHECKOUT", 'E');
	printFormattedOption("RETURN", 'F');
	printFormattedOption("EXIT", 'Q');
}

void promptPublicContents() {
	printFormattedTitle("LIST OF AVAILABLE ITEMS");
	printBookshelfContents(publicBookshelf);
	getCharacterInput(">>");
}

void promptMyContents() {
	printFormattedTitle("LIST OF MY ITEMS");
	printBookshelfContents(myBookshelf);
	getCharacterInput(">>");
}

void promptAddition() {
	printFormattedTitle("ITEM ADDITION PROMPT");

	BOOK* book = createEmptyBook();

	getInputInDefaultBuffer("TITLE: ", NULL, false);
	setTitle(book, defaultBuffer);

	getInputInDefaultBuffer("AUTHOR: ", NULL, false);
	setAuthor(book, defaultBuffer);

	setPages(book, getNumberInput("PAGES: ", 0, INT_MAX));

	if (!addBook(publicBookshelf, book)) {
		printf("MAX CAPACITY REACHED: UNABLE TO ADD NEW ITEM\n");
		freeBook(book);
	}
	else {
		printf("ITEM SUCCESSFULLY ADDED\n");
	}
}

void promptDeletion() {
	printFormattedTitle("ITEM DELETION PROMPT");
	printBookshelfContents(publicBookshelf);

	int deleteIndex = getNumberInput("INDEX: ", 0, LIBRARY_CAPACITY);

	if (!deleteBook(publicBookshelf, deleteIndex)) {
		printf("INVALID ITEM: UNABLE TO DELETE NON-EXISTING ITEM\n");
	}
	else {
		printf("ITEM SUCCESSFULLY DELETED\n");
	}
}

void promptCheckout() {
	printFormattedTitle("ITEM CHECKOUT PROMPT");
	printBookshelfContents(publicBookshelf);

	int reservationIndex = getNumberInput("INDEX: ", 0, LIBRARY_CAPACITY);
	BOOK* bookToReserve = takeBook(publicBookshelf, reservationIndex);

	if (bookToReserve == NULL) {
		printf("INVALID ITEM: CANNONT CHECKOUT A NON-EXISTING ITEM\n");
	}
	else {
		addBook(myBookshelf, bookToReserve);
		printf("ITEM SUCCESSFULLY RESERVED\n");
	}
}

void promptReturn() {
	printFormattedTitle("ITEM RETURN PROMPT");
	printBookshelfContents(myBookshelf);

	int returnIndex = getNumberInput("INDEX: ", 0, LIBRARY_CAPACITY);
	BOOK* bookToReturn = takeBook(myBookshelf, returnIndex);

	if (bookToReturn == NULL) {
		printf("INVALID ITEM: CANNONT RETURN A NON-EXISTING ITEM\n");
	}
	else {
		addBook(publicBookshelf, bookToReturn);
		printf("ITEM SUCCESSFULLY RETURNED\n");
	}
}

void promptExit() {
	char save = getCharacterInput("SAVE BEFORE EXIT? (Y/N): ");

	switch (save) {
	case 'Y':
		writeBookshelfToFile(publicBookshelf, PUBLIC_BOOKSHELF_FILENAME);
		writeBookshelfToFile(myBookshelf, MY_BOOKSHELF_FILENAME);
		break;
	case 'N':
		break;
	default:
		printf("'%c' IS NOT AN IDENTIFIABLE COMMAND\n", save);
		return;
	}

	exit(0);
}

void promptMainMenu() {
	char optionSelection = 0;
	int exitMainPrompt = 0;

	while (!exitMainPrompt) {
		printMainOptions();
		optionSelection = getCharacterInput(">>");

		switch (optionSelection) {
		case 'A': promptPublicContents(); break;
		case 'B': promptAddition(); break;
		case 'C': promptDeletion(); break;
		case 'D': promptMyContents(); break;
		case 'E': promptCheckout(); break;
		case 'F': promptReturn(); break;
		case 'Q': promptExit(); break;
		default:
			printf("'%c' IS NOT AN IDENTIFIABLE COMMAND\n", optionSelection);
			break;
		}
	}
}

void setupBookshelves() {
	publicBookshelf = readBookshelfFromFile(PUBLIC_BOOKSHELF_FILENAME);
	myBookshelf = readBookshelfFromFile(MY_BOOKSHELF_FILENAME);

	if (publicBookshelf == NULL) {
		publicBookshelf = createEmptyBookshelf(LIBRARY_CAPACITY);

		//adding default books
		addBook(publicBookshelf, createBook("Head First C", "David Griffiths", 633));
		addBook(publicBookshelf, createBook("Head First Java", "Kathy Sierra", 722));
		addBook(publicBookshelf, createBook("Prime Obsession", "John Derbyshire", 447));
		addBook(publicBookshelf, createBook("Fermat's Last Theorem", "Simon Singh", 368));
	}

	if (myBookshelf == NULL)
		myBookshelf = createEmptyBookshelf(LIBRARY_CAPACITY);
}

int main(void) {
	printf(">>LIBRARY SYSTEM: VERSION %d\n", VERSION_ID);

	promptAccountMenu();
	setupBookshelves();
	promptMainMenu();
	return 0;
}
