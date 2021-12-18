#include <stdlib.h>
#include <time.h>

int getRandomNumber(int lowerBound, int upperBound) {
	//lowerBound and upperBound are both inclusive.

	srand(time(NULL));
	return rand() % (upperBound - lowerBound + 1) + lowerBound;
}

char getRandomAlphabet() {
	//returns a random 'lowercase' alphabet character
	return 'a' + getRandomNumber(0, 25);
}