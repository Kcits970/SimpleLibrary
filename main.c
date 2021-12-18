#define _CRT_SECURE_NO_WARNINGS
#define VERSION_ID 4
#define LIBRARY_CAPACITY 8

#include <stdio.h>
#include <stdlib.h>

#include "userinput.h"
#include "formatting.h"
#include "book.h"
#include "account.h"

BOOKSHELF* publicBookshelf = NULL;
BOOKSHELF* myBookshelf = NULL;

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

	getInputInDefaultBuffer("TITLE: ");
	setTitle(book, defaultBuffer);

	getInputInDefaultBuffer("AUTHOR: ");
	setAuthor(book, defaultBuffer);

	setPages(book, getNumberInput("PAGES: ", 0, INT_MAX));

	if (!addBook(publicBookshelf, book)) {
		printf("MAX CAPACITY REACHED: UNABLE TO ADD NEW ITEM\n");
		freeBook(book);
	} else {
		printf("ITEM SUCCESSFULLY ADDED\n");
	}
}

void promptDeletion() {
	printFormattedTitle("ITEM DELETION PROMPT");
	printBookshelfContents(publicBookshelf);

	int deleteIndex = getNumberInput("INDEX: ", 0, LIBRARY_CAPACITY);

	if (!deleteBook(publicBookshelf, deleteIndex)) {
		printf("INVALID ITEM: UNABLE TO DELETE NON-EXISTING ITEM\n");
	} else {
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
	} else {
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
	} else {
		addBook(publicBookshelf, bookToReturn);
		printf("ITEM SUCCESSFULLY RETURNED\n");
	}
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
		case 'Q':
			exitMainPrompt = 1;
			break;
		default:
			printf("'%c' IS NOT AN IDENTIFIABLE COMMAND\n", optionSelection);
			break;
		}
	}
}

int main(void) {
	printf(">>LIBRARY SYSTEM: VERSION %d\n", VERSION_ID);

	publicBookshelf = createEmptyBookshelf(LIBRARY_CAPACITY);
	myBookshelf = createEmptyBookshelf(LIBRARY_CAPACITY);

	//adding default books
	addBook(publicBookshelf, createBook("Head First C", "David Griffiths", 633));
	addBook(publicBookshelf, createBook("Head First Java", "Kathy Sierra", 722));
	addBook(publicBookshelf, createBook("Prime Obsession", "John Derbyshire", 447));
	addBook(publicBookshelf, createBook("Fermat's Last Theorem", "Simon Singh", 368));

	promptAccountMenu();
	promptMainMenu();
	return 0;
}