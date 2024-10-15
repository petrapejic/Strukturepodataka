/* 1. ZADATAK */

/*Napisati program koji prvo proèita koliko redaka ima datoteka, tj. koliko ima studenata
zapisanih u datoteci. Nakon toga potrebno je dinamièki alocirati prostor za niz struktura
studenata (ime, prezime, bodovi) i uèitati iz datoteke sve zapise. Na ekran ispisati ime,
prezime, apsolutni i relativni broj bodova.
Napomena: Svaki redak datoteke sadrži ime i prezime studenta, te broj bodova na kolokviju.
relatvan_br_bodova = br_bodova/max_br_bodova*100
*/

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ERROR_OPENING_FILE 1
#define BUFFER_SIZE 1024
#define max_br_bodova 100

typedef struct _student {
	char ime[50];
	char prezime[50];
	float bod;
}Student;

int count_rows(char* filename);
void read_stud(char* filename, Student* studenti, int broj_stud);
void print_stud(Student* studenti, int broj_stud);

int main()
{
	int broj_stud = 0;
	Student* studenti = NULL;
	char filename[] = "studenti.txt";

	//broj redaka u datoteci
	broj_stud = count_rows(filename);

	//alokacija memorije
	studenti = (Student*)malloc(broj_stud * sizeof(Student));
	if (studenti == NULL) {
		printf("Greska pri alokaciji memorije!");
		return -1;
	}
	
	//citanje podataka
	read_stud(filename, studenti, broj_stud);

	//ispis
	print_stud(studenti, broj_stud);

	//osloboditi memoriju
	free(studenti);

	return 0;
}

int count_rows(char* filename)
{
	FILE* fp = NULL;
	char buffer[BUFFER_SIZE];
	int row_count = 0;

	fp = fopen(filename, "r");

	if (fp == NULL) {
		printf("Error opening file!");
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

void read_stud(char* filename, Student* studenti, int broj_stud)
{
	FILE* fp = NULL;
	int i = 0;

	fp = fopen(filename, "r");

	if (fp == NULL) {
		printf("Error opening file!");
		return ERROR_OPENING_FILE;
	}

	for (i = 0; i < broj_stud; i++) {
		fscanf(fp, "%s %s %f", studenti[i].ime, studenti[i].prezime, &studenti[i].bod);
	}

	fclose(fp);

	return;
}

void print_stud(Student* studenti, int broj_stud)
{
	int i = 0;
	float relatvan_br_bodova;

	for (i = 0; i < broj_stud; i++) {
		relatvan_br_bodova = studenti[i].bod / max_br_bodova * 100;

		printf("Ime i prezime: %s %s, Bodovi: %.2f, Relativni bodovi: %.2f\n", studenti[i].ime, studenti[i].prezime, studenti[i].bod, relatvan_br_bodova);
	}

	return;
}