#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE 1024

#define USPJEH 0
#define POGRESKA_OTVARANJE_DATOTEKE -1
#define POGRESKA_MALLOC -2
#define POGRESKA_SCANF -3

struct clanak; // struktura za clanak
typedef struct clanak* PozicijaClanka; // tip za poziciju clanka
typedef struct clanak {
    char ime[20];  // ime artikla
    int kolicina;   // kolicina artikla
    int cijena;      // cijena artikla
    PozicijaClanka sljedeci;  // sljedeci clanak
} Clanak;

struct racun; // struktura za racun
typedef struct racun* PozicijaRacuna; // tip za poziciju racuna
typedef struct racun {
    char datum[10];  // datum racuna
    Clanak glavniClanak;  // prvi clanak na racunu
    PozicijaRacuna sljedeci;  // sljedeci racun
} Racun;

int dodajClanak(PozicijaClanka glava, PozicijaClanka clanak); // dodaj clanak
int dodajRacun(PozicijaRacuna glava, PozicijaRacuna racun); // dodaj racun
int procitajRacun(char* imeDatoteke, PozicijaRacuna glavaRacuna); // procitaj racun iz datoteke
int ispisiClanke(PozicijaClanka clanak); // ispisi clanke
int ispisiRacune(PozicijaRacuna racun); // ispisi racune
int dohvatiClanakURazdoblju(PozicijaRacuna trenutniRacun, char* clanak, char* pocetniDatum, char* zavrsniDatum, int* prihod, int* kolicina); // dohvati clanak u periodu
int obrisiClanke(PozicijaClanka glavaClanka); // obrisi clanke
int obrisiRacune(PozicijaRacuna glavaRacuna); // obrisi racune

int main()
{
    int prihod = 0, kolicina = 0;
    char clanak[100] = { 0 };
    char pocetniDatum[20] = { 0 }, zavrsniDatum[20] = { 0 };
    char trenutniRacun[MAX_LINE] = { 0 };
    FILE* pokazivacNaDatoteku = NULL;
    Racun glavaRacuna = {
        .datum = {0},
        .glavniClanak = NULL,
        .sljedeci = NULL
    };

    pokazivacNaDatoteku = fopen("racuni.txt", "r"); // otvori datoteku za citanje
    if (pokazivacNaDatoteku == NULL) {
        printf("Datoteka se nije mogla otvoriti.\n");
        return POGRESKA_OTVARANJE_DATOTEKE;
    }

    while (!feof(pokazivacNaDatoteku)) {
        fgets(trenutniRacun, MAX_LINE, pokazivacNaDatoteku);
        trenutniRacun[strcspn(trenutniRacun, "\n")] = 0; // ukloni '\n'
        printf("%s\n", trenutniRacun);
        procitajRacun(trenutniRacun, &glavaRacuna); // procitaj racun
    }

    ispisiRacune(glavaRacuna.sljedeci); // ispisi racune

    printf("\nUnesite ime artikla: ");
    if (scanf("%s", clanak) != 1) return POGRESKA_SCANF; // unesite ime artikla
    printf("Unesite pocetni datum (YYYY-MM-DD): ");
    if (scanf("%s", pocetniDatum) != 1) return POGRESKA_SCANF; // unesite pocetni datum
    printf("Unesite zavrsni datum (YYYY-MM-DD): ");
    if (scanf("%s", zavrsniDatum) != 1) return POGRESKA_SCANF; // unesite zavrsni datum

    dohvatiClanakURazdoblju(&glavaRacuna, clanak, pocetniDatum, zavrsniDatum, &prihod, &kolicina); // dohvati clanak u razdoblju
    printf("Prihod od %d artikla %s: %d\n", kolicina, clanak, prihod);

    obrisiRacune(&glavaRacuna); // obrisi racune
    fclose(pokazivacNaDatoteku); // zatvori datoteku

    return 0;
}

int dodajClanak(PozicijaClanka glava, PozicijaClanka clanak)
{
    PozicijaClanka trenutni = glava;

    while (trenutni->sljedeci != NULL && strcmp(trenutni->sljedeci->ime, clanak->ime) == -1)
        trenutni = trenutni->sljedeci;

    clanak->sljedeci = trenutni->sljedeci;
    trenutni->sljedeci = clanak;

    return 0;
}

int dodajRacun(PozicijaRacuna glava, PozicijaRacuna racun)
{
    PozicijaRacuna trenutni = glava;

    while (trenutni->sljedeci != NULL && strcmp(trenutni->sljedeci->datum, racun->datum) <= 0)
        trenutni = trenutni->sljedeci;

    racun->sljedeci = trenutni->sljedeci;
    trenutni->sljedeci = racun;

    return 0;
}

int procitajRacun(char* imeDatoteke, PozicijaRacuna glavaRacuna)
{
    char datumString[20] = { 0 };
    char clanakString[MAX_LINE] = { 0 };
    int kolicina = 0;
    FILE* pokazivacNaDatoteku = NULL;
    PozicijaRacuna noviRacun = NULL;
    PozicijaClanka noviClanak = NULL;

    pokazivacNaDatoteku = fopen(imeDatoteke, "r"); // otvori datoteku za citanje
    if (pokazivacNaDatoteku == NULL) {
        printf("Datoteka se nije mogla otvoriti.\n");
        return POGRESKA_OTVARANJE_DATOTEKE;
    }

    noviRacun = (PozicijaRacuna)malloc(sizeof(Racun));
    if (noviRacun == NULL) {
        printf("Pogreska pri alokaciji memorije.\n");
        return POGRESKA_MALLOC;
    }

    fgets(datumString, 20, pokazivacNaDatoteku);
    datumString[strcspn(datumString, "\n")] = 0;

    strcpy(noviRacun->datum, datumString);
    noviRacun->glavniClanak.sljedeci = NULL;

    while (!feof(pokazivacNaDatoteku))
    {
        noviClanak = (PozicijaClanka)malloc(sizeof(Clanak));
        if (noviClanak == NULL) {
            printf("Pogreska pri alokaciji memorije.\n");
            return POGRESKA_MALLOC;
        }

        if (fscanf(pokazivacNaDatoteku, "%[^,], %d, %d ",
            noviClanak->ime, &noviClanak->kolicina, &noviClanak->cijena) != 3) {
            printf("Pogreska pri citanju s datoteke.\n");
            return POGRESKA_SCANF;
        }

        dodajClanak(&noviRacun->glavniClanak, noviClanak); // dodaj clanak na racun
    }

    dodajRacun(glavaRacuna, noviRacun); // dodaj racun u listu

    fclose(pokazivacNaDatoteku);

    return 0;
}

int ispisiClanke(PozicijaClanka trenutni)
{
    while (trenutni != NULL) {
        printf("\t%s, %d, %d\n", trenutni->ime, trenutni->kolicina, trenutni->cijena); // ispisi clanak
        trenutni = trenutni->sljedeci;
    }

    return 0;
}

int ispisiRacune(PozicijaRacuna trenutni)
{
    while (trenutni != NULL) {
        printf("Racun od datuma %s:\n", trenutni->datum); // ispisi datum racuna
        ispisiClanke(trenutni->glavniClanak.sljedeci); // ispisi artikle za racun
        trenutni = trenutni->sljedeci;
    }

    return 0;
}

int dohvatiClanakURazdoblju(PozicijaRacuna trenutniRacun, char* clanak, char* pocetniDatum, char* zavrsniDatum, int* prihod, int* kolicina)
{
    PozicijaClanka trenutniClanak = NULL;

    // Premjesti na prvi racun s datumom vecim ili jednakim pocetnom datumu
    while (trenutniRacun != NULL && strcmp(trenutniRacun->datum, pocetniDatum) == -1)
        trenutniRacun = trenutniRacun->sljedeci;

    while (trenutniRacun != NULL && strcmp(trenutniRacun->datum, zavrsniDatum) <= 0) {
        trenutniClanak = trenutniRacun->glavniClanak.sljedeci;
        while (trenutniClanak != NULL) {
            if (strcmp(trenutniClanak->ime, clanak) == 0) { // ako je ime artikla isto
                *prihod += trenutniClanak->kolicina * trenutniClanak->cijena; // izracunaj prihod
                *kolicina += trenutniClanak->kolicina; // zbroji kolicinu
            }
            trenutniClanak = trenutniClanak->sljedeci;
        }
        trenutniRacun = trenutniRacun->sljedeci;
    }

    return USPJEH;
}


int obrisiClanke(PozicijaClanka glavaClanka)
{
    PozicijaClanka temp = NULL;

    while (glavaClanka->sljedeci != NULL) {
        temp = glavaClanka->sljedeci;
        glavaClanka->sljedeci = temp->sljedeci;
        free(temp);
    }

    return 0;
}

int obrisiRacune(PozicijaRacuna glavaRacuna)
{
    PozicijaRacuna temp = NULL;

    while (glavaRacuna->sljedeci != NULL) {
        temp = glavaRacuna->sljedeci;
        glavaRacuna->sljedeci = temp->sljedeci;
        obrisiClanke(&temp->glavniClanak);
        free(temp);
    }

    return 0;
}
