typedef struct {
	char* title;
	char* author;
	int pages;
} BOOK;

BOOK* createEmptyBook() {
	BOOK* book = malloc(sizeof(BOOK));
	book->title = NULL;
	book->author = NULL;
	book->pages = 0;

	return book;
}

void setTitle(BOOK* book, const char* title) {
	book->title = getEmptyString(strlen(title) + 1);
	strcpy(book->title, title);
}

void setAuthor(BOOK* book, const char* author) {
	book->author = getEmptyString(strlen(author) + 1);
	strcpy(book->author, author);
}

void setPages(BOOK* book, int pages) {
	book->pages = pages;
}

BOOK* createBook(const char* title, const char* author, int pages) {
	BOOK* book = createEmptyBook();
	setTitle(book, title);
	setAuthor(book, author);
	setPages(book, pages);

	return book;
}

void freeBook(BOOK* book) {
	free(book->title);
	free(book->author);
	free(book);
}

typedef struct {
	int capacity;
	BOOK** bookHeap;
	int currentSize;
} BOOKSHELF;

BOOKSHELF* createEmptyBookshelf(int capacity) {
	BOOKSHELF* bookshelf = malloc(sizeof(BOOKSHELF));
	bookshelf->capacity = capacity;
	bookshelf->currentSize = 0;
	bookshelf->bookHeap = malloc(capacity * sizeof(BOOK*));

	for (int i = 0; i < capacity; i++)
		bookshelf->bookHeap[i] = NULL;

	return bookshelf;
}

int searchAvailableIndex(BOOKSHELF* bookshelf) {
	for (int i = 0; i < bookshelf->capacity; i++)
		if (bookshelf->bookHeap[i] == NULL)
			return i;

	return -1;
}

int searchNthBookIndex(BOOKSHELF* bookshelf, int nth) {
	int booksFound = 0;

	for (int i = 0; i < bookshelf->capacity; i++) {
		BOOK* currentBook = bookshelf->bookHeap[i];
		if (currentBook != NULL && ++booksFound == nth)
			return i;
	}

	return -1;
}

int addBook(BOOKSHELF* bookshelf, BOOK* book) {
	int availableIndex = searchAvailableIndex(bookshelf);

	if (availableIndex != -1) {
		bookshelf->bookHeap[availableIndex] = book;
		(bookshelf->currentSize)++;
		return 1;
	} else {
		return 0;
	}
}

BOOK* takeBook(BOOKSHELF* bookshelf, int nth) {
	int nthBookIndex = searchNthBookIndex(bookshelf, nth);

	if (nthBookIndex != -1) {
		BOOK* nthBook = bookshelf->bookHeap[nthBookIndex];
		bookshelf->bookHeap[nthBookIndex] = NULL;
		(bookshelf->currentSize)--;
		return nthBook;
	}

	return NULL;
}

int deleteBook(BOOKSHELF* bookshelf, int nth) {
	int nthBookIndex = searchNthBookIndex(bookshelf, nth);

	if (nthBookIndex != -1) {
		freeBook(bookshelf->bookHeap[nthBookIndex]);
		bookshelf->bookHeap[nthBookIndex] = NULL;
		(bookshelf->currentSize)--;
		return 1;
	}

	return 0;
}

void printCriteriaNames() {
	printNCharacters("INDEX", 5);
	printf(" | ");
	printNCharacters("TITLE", 16);
	printf(" | ");
	printNCharacters("AUTHOR", 16);
	printf(" | ");
	printNCharacters("PAGES", 5);
	printf("\n");
}

void printBookshelfContents(BOOKSHELF* bookshelf) {
	if (bookshelf->currentSize == 0) return;

	int booksFound = 0;
	printCriteriaNames();

	for (int i = 0; i < bookshelf->capacity; i++) {
		BOOK* currentBook = bookshelf->bookHeap[i];
		if (currentBook != NULL) {
			printNDigits(++booksFound, 5);
			printf(" | ");
			printNCharacters(currentBook->title, 16);
			printf(" | ");
			printNCharacters(currentBook->author, 16);
			printf(" | ");
			printNDigits(currentBook->pages, 5);
			printf("\n");
		}
	}

}