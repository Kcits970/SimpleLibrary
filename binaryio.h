#ifndef BINARYIO_H_
#define BINARYIO_H_

void writeInteger(FILE* fp, int n) {
	fwrite(&n, sizeof(int), 1, fp);
}

void writeCharArray(FILE* fp, const char* chars, int arraySize) {
	writeInteger(fp, arraySize);
	fwrite(chars, sizeof(char), arraySize, fp);
}

void writeString(FILE* fp, const char* str) {
	writeCharArray(fp, str, strlen(str)+1);
}

bool readInteger(FILE* fp, int* n) {
	if (fread(n, sizeof(int), 1, fp) == 1)
		return true;
	else
		return false;
}

bool readCharArray(FILE* fp, char* buffer, int bufferLimit) {
	int arraySize;
	if (!readInteger(fp, &arraySize)) return false;

	bufferLimit = (arraySize > bufferLimit) ? bufferLimit : arraySize;

	if (fread(buffer, sizeof(char), bufferLimit, fp) != bufferLimit)
		return false;
	else
		return true;
}

bool readString(FILE* fp, char* buffer, int bufferLimit) {
	if (readCharArray(fp, buffer, &bufferLimit)) {
		buffer[bufferLimit - 1] = '\0';
		return true;
	}
	else
		return false;
}

#endif
