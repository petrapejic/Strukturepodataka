/*Napisati program za zbrajanje i mnozenje polinoma. Koeficijenti i eksponenti se
citaju iz datoteke.
Napomena: Eksponenti u datoteci nisu nuzno sortirani.*/

#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>

#define MAX_LINE 1024
#define MAX_SIZE 50

typedef struct _Element* Position;
typedef struct _Element {
	int coeff;
	int exp;

	Position next;
}Element;

int UnesiNazivDatoteke(char* filename);
int UcitajDatoteku(Position head1, Position head2, char* filename);
int ParsirajStringUListu(Position head, char* buffer);
int ObrisiNakon(Position previous);
int UnesiNakon(Position previous, Position newElement);
int UnesiSortirano(Position previous, Position newElement);
int SpojiNakon(Position position, Position newElement);
Position KreirajElement(int coeff, int exp);
int IspisiPolinom(char* name, Position first);
int KreirajIUnesiNakon(int coeff, int exp, Position position);
int ZbrojiPolinome1(Position resultHead, Position head1, Position head2);
int ZbrojiPolinome2(Position resultHead, Position head1, Position head2);
int PomnoziPolinome(Position resultHead, Position head1, Position head2);
int OslobodiMemoriju(Position head);

int main()
{
	Element head1 = { .coeff = 0, .exp = 0, .next = NULL };
	Element head2 = { .coeff = 0, .exp = 0, .next = NULL };
	Element rezultatZbroj = { .coeff = 0, .exp = 0, .next = NULL };
	Element rezultatUmnozak = { .coeff = 0, .exp = 0, .next = NULL };
	char fileName[MAX_SIZE] = { 0 };

	UnesiNazivDatoteke(fileName);

	if (UcitajDatoteku(&head1, &head2, fileName) == EXIT_SUCCESS) {
		IspisiPolinom("pol1", head1.next);
		IspisiPolinom("pol2", head2.next);

		ZbrojiPolinome1(&rezultatZbroj, &head1, &head2);
		PomnoziPolinome(&rezultatUmnozak, &head1, &head2);

		IspisiPolinom("polZbroj", rezultatZbroj.next);
		IspisiPolinom("polUmnozak", rezultatUmnozak.next);

		OslobodiMemoriju(&head1);
		OslobodiMemoriju(&head2);
		OslobodiMemoriju(&rezultatZbroj);
		OslobodiMemoriju(&rezultatUmnozak);
	}
	return EXIT_SUCCESS;
}

// Funkcija koja unosi naziv datoteke
int UnesiNazivDatoteke(char* filename)
{
	printf("Unesi naziv datoteke:\n");
	scanf(" %s", filename);

	return EXIT_SUCCESS;
}

// Funkcija koja cita datoteku i sprema podatke u liste
int UcitajDatoteku(Position head1, Position head2, char* filename)
{
	FILE* file = NULL;
	char buffer[MAX_LINE] = { 0 };
	int status = EXIT_SUCCESS;

	file = fopen(filename, "r");
	if (!file) {
		printf("Greska pri otvaranju datoteke!\n");
		return -1;
	}

	fgets(buffer, MAX_LINE, file);
	status = ParsirajStringUListu(head1, buffer);
	if (status != EXIT_SUCCESS) {
		return EXIT_FAILURE;
	}

	fgets(buffer, MAX_LINE, file);
	status = ParsirajStringUListu(head2, buffer);
	if (status != EXIT_SUCCESS) {
		return EXIT_FAILURE;
	}

	fclose(file);

	return EXIT_SUCCESS;
}

// Funkcija koja parsira string i doadje monome u listu
int ParsirajStringUListu(Position head, char* buffer)
{
	char* trenutniBuffer = buffer;
	int coefficient = 0;
	int exponent = 0;
	int numBytes = 0;
	int status = 0;
	Position newElement = NULL;

	while (strlen(trenutniBuffer) > 0) {
		status = sscanf(trenutniBuffer, " %d %d %n", &coefficient, &exponent, &numBytes);
		if (status != 2) {
			printf("Greska u formatu datoteke!\n");
			return EXIT_FAILURE;
		}
		newElement = KreirajElement(coefficient, exponent);
		if (!newElement) {
			return EXIT_FAILURE;
		}

		UnesiSortirano(head, newElement);
		trenutniBuffer += numBytes;
	}
	return EXIT_SUCCESS;
}

// Funkcija koja brise element liste iza danog elementa
int ObrisiNakon(Position previous)
{
	Position toDelete = previous->next;
	previous->next = toDelete->next;
	free(toDelete);

	return EXIT_SUCCESS;
}

// Funkcija koja unosi element nakon danog polozaja u listi
int UnesiNakon(Position previous, Position newElement)
{
	newElement->next = previous->next;
	previous->next = newElement;

	return EXIT_SUCCESS;
}

int UnesiSortirano(Position head, Position newElement)
{
	Position temp = head;

	while (temp->next != NULL && temp->next->exp < newElement->exp) {
		temp = temp->next;
	}
	SpojiNakon(temp, newElement);

	return EXIT_SUCCESS;
}

// Funkcija koja spaja dva monoma ako imaju isti exsponent ili dodaje novi monom u listu
int SpojiNakon(Position position, Position newElement)
{
	if (position->next == NULL || position->next->exp != newElement->exp) {
		UnesiNakon(position, newElement);
	}
	else {
		int rezultatKoeficijent = position->next->coeff + newElement->coeff;
		if (rezultatKoeficijent == 0) {
			ObrisiNakon(position);
		}
		else {
			position->next->coeff = rezultatKoeficijent;
		}
		free(newElement);
	}
	return EXIT_SUCCESS;
}

// Funkcija koja kreira novi element (monom) s koeficijentom i eksponentom
Position KreirajElement(int coeff, int exp)
{
	Position element = (Position)malloc(sizeof(Element));

	if (!element) {
		printf("Memorija se ne moze alocirati!\n");
		return element;
	}

	element->coeff = coeff;
	element->exp = exp;
	element->next = NULL;

	return element;
}

// Funkcija koja ispisuje polinome
int IspisiPolinom(char* name, Position first)
{
	printf(" %s= ", name);
	if (first) {
		if (first->exp < 0) {
			if (first->coeff == 1) {
				printf("x^(%d)", first->exp);
			}
			else {
				printf("%dx^(%d)", first->coeff, first->exp);
			}
		}
		else {
			if (first->coeff == 1) {
				printf("x^(%d)", first->exp);
			}
			else {
				printf("%dx^(%d)", first->coeff, first->exp);
			}
		}
		first = first->next;
	}

	for (; first != NULL; first = first->next) {
		if (first->coeff < 0) {
			if (first->exp < 0) {
				printf(" -%dx^(%d)", abs(first->coeff), first->exp);
			}
			else {
				printf(" -%dx^%d", abs(first->coeff), first->exp);
			}
		}
		else {
			if (first->exp < 0) {
				printf(" +%dx^(%d)", first->coeff, first->exp);
			}
			else {
				printf(" +%dx^%d", first->coeff, first->exp);
			}
		}
	}
	printf("\n");

	return EXIT_SUCCESS;
}

// Funkcija koja stvara element i umece ga iza daog polozaja
int KreirajIUnesiNakon(int coeff, int exp, Position position)
{
	Position newElement = KreirajElement(coeff, exp);
	if (!newElement) {
		return EXIT_FAILURE;
	}

	UnesiNakon(position, newElement);

	return EXIT_SUCCESS;
}

// Funkcija koja zbraja dva polinoma 1. nacin
int ZbrojiPolinome1(Position resultHead, Position head1, Position head2)
{
	Position i = head1->next;
	Position j = head2->next;
	Position k = resultHead;
	Position temp = NULL;

	while (i != NULL && j != NULL) {
		if (i->exp == j->exp) {
			KreirajIUnesiNakon(i->coeff + j->coeff, i->exp, k);
			i = i->next;
			j = j->next;
		}
		else if (i->exp < j->exp) {
			KreirajIUnesiNakon(i->coeff, i->exp, k);
			i = i->next;
		}
		else {
			KreirajIUnesiNakon(j->coeff, j->exp, k);
			j = j->next;
		}
	}

	temp = (i == NULL) ? j : i;
	while (temp != NULL) {
		KreirajIUnesiNakon(temp->coeff, temp->exp, k);
		temp = temp->next;
	}

	return EXIT_SUCCESS;
}

// Funkcija koja zbraja dva polinoma 2. nacin
int ZbrojiPolinome2(Position resultHead, Position head1, Position head2)
{
	Position i, j;

	for (i = head1->next; i != NULL; i = i->next) {
		Position newElement = KreirajElement(i->coeff, i->exp);
		if (!newElement) {
			return EXIT_FAILURE;
		}
		UnesiSortirano(resultHead, newElement);
	}

	for (j = head2->next; j != NULL; j = j->next) {
		Position newElement = KreirajElement(j->coeff, j->exp);
		if (!newElement) {
			return EXIT_FAILURE;
		}
		UnesiSortirano(resultHead, newElement);
	}

	return EXIT_SUCCESS;
}

// Funkcija koja mnozi dva polinoma
int PomnoziPolinome(Position resultHead, Position head1, Position head2)
{
	Position i, j;

	for (i = head1->next; i != NULL; i = i->next) {
		for (j = head2->next; j != NULL; j = j->next) {
			Position newElement = KreirajElement(i->coeff * j->coeff, i->exp + j->exp);
			if (!newElement) {
				return EXIT_FAILURE;
			}
			UnesiSortirano(resultHead, newElement);
		}
	}
	return EXIT_SUCCESS;
}

int OslobodiMemoriju(Position head)
{
	Position temp = head;

	while (temp->next) {
		ObrisiNakon(temp);
	}

	return EXIT_SUCCESS;
}