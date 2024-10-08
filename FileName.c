/* 1. ZADATAK */
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#define ERROR_OPENING_FILE 1
#define BUFFER_SIZE 1024

int count_rows( char* filename);


int main() {
	

	return 0;
}

int count_rows(char* filename) {
	FILE* fp = NULL; 
	char buffer[BUFFER_SIZE];
	int row_count = 0;

	fp = fopen(filename, "r");
	if (fp == NULL) {
		printf("Error opening file!\n");
		return ERROR_OPENING_FILE;
	}

	while (!feof(fp)) {
		fgets(buffer, BUFFER_SIZE, fp);
		// sscanf() - domaci rad 
		++row_count;
	}

	fclose(fp);
	return row_count;
}