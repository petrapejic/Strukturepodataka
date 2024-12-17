
#define _CRT_SECURE_NO_WARNINGS


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NAZIV 100
#define BUFFER_SIZE 1024

typedef struct Direktorij* direktorij;
typedef struct Direktorij {
    char naziv[MAX_NAZIV];
    direktorij sljedeci;
    direktorij prethodni;
    direktorij roditelj;
} Direktorij;

direktorij kreirajDirektorij(const char* naziv, direktorij roditelj) {
    direktorij novi = (direktorij)malloc(sizeof(Direktorij));
    if (novi == NULL) {
        printf("Greska pri alociranju memorije!\n");
        exit(1);
    }

    strcpy(novi->naziv, naziv);
    novi->sljedeci = NULL;
    novi->prethodni = NULL;
    novi->roditelj = roditelj;

    return novi;
}

void dodajPodDirektorij(direktorij trenutni, const char* naziv) {
    direktorij novi = kreirajDirektorij(naziv, trenutni);

    if (trenutni != NULL) {
        direktorij temp = trenutni;
        while (temp->sljedeci != NULL) {
            temp = temp->sljedeci;
        }
        temp->sljedeci = novi;
        novi->prethodni = temp;
    }
    else {
        trenutni = novi;
    }
}

direktorij pronadjiPodDirektorij(direktorij trenutni, const char* naziv) {
    direktorij temp = trenutni->sljedeci;

    while (temp != NULL) {
        if (strcmp(temp->naziv, naziv) == 0) {
            return temp;
        }
        temp = temp->sljedeci;
    }

    return NULL;
}

void ispisDirektorija(direktorij trenutni) {
    direktorij temp = trenutni->sljedeci;
    if (temp == NULL) return;

    while (temp != NULL) {
        printf("%s\n", temp->naziv);
        temp = temp->sljedeci;
    }
}

direktorij cdUp(direktorij trenutni) {
    if (trenutni->roditelj != NULL) {
        return trenutni->roditelj;
    }
    return trenutni;
}

void oslobodiDirektorij(direktorij trenutni) {
    if (trenutni == NULL) return;

    direktorij temp = trenutni->sljedeci;
    while (temp != NULL) {
        direktorij to_delete = temp;
        temp = temp->sljedeci;
        oslobodiDirektorij(to_delete);
    }
    free(trenutni);
}

void unosKomande(direktorij* trenutni) {
    int izbor;
    char naziv[MAX_NAZIV];
    char komanda[MAX_NAZIV];
    char buffer[BUFFER_SIZE];

    while (1) {
        printf("\nMeni:\n");
        printf("- md [naziv]\n");
        printf("- cd [naziv]\n");
        printf("- cd..\n");
        printf("- dir\n");
        printf("- Izlaz\n");
        printf("\nTrenutni direktorij: %s\n", (*trenutni)->naziv);
        printf("\nUnesite komandu: ");

        if (fgets(buffer, MAX_NAZIV, stdin) == NULL) {
            printf("Greska prilikom citanja komande!\n");
            continue;
        }
        buffer[strcspn(buffer, "\n")] = 0;

        int argumenti = sscanf(buffer, "%s %s", komanda, naziv);
        if (argumenti == -1) continue;

        if (strcmp(komanda, "md") == 0) {
            if (argumenti < 2) {
                printf("\nNedostaje naziv za 'md'.\n");
                continue;
            }

            if (pronadjiPodDirektorij(*trenutni, naziv) != NULL) {
                printf("\nDirektorij '%s' vec postoji.\n", naziv);
                continue;
            }

            dodajPodDirektorij(*trenutni, naziv);
            printf("\nDirektorij '%s' uspjesno kreiran.\n", naziv);

        }
        else if (strcmp(komanda, "cd") == 0) {
            if (argumenti < 2) {
                printf("\nNedostaje naziv za 'cd'.\n");
                continue;
            }

            direktorij podDirektorij = pronadjiPodDirektorij(*trenutni, naziv);
            if (podDirektorij != NULL) {
                *trenutni = podDirektorij;
                printf("\nUsli ste u direktorij '%s'.\n", naziv);
            }
            else {
                printf("\nPod-direktorij '%s' ne postoji.\n", naziv);
            }

        }
        else if (strcmp(komanda, "cd..") == 0) {
            *trenutni = cdUp(*trenutni);
            printf("\nVratili ste se u roditeljski direktorij.\n");

        }
        else if (strcmp(komanda, "dir") == 0) {
            printf("\nSadrzaj direktorija '%s':\n", (*trenutni)->naziv);
            ispisDirektorija(*trenutni);

        }
        else if (strcmp(komanda, "izlaz") == 0 || strcmp(buffer, "izlaz") == 0) {
            printf("\nIzlaz iz programa.\n");
            break;
        }
        else {
            printf("\nNepoznata komanda: '%s'\n", komanda);
        }
    }
}

int main() {
    direktorij root = (direktorij)malloc(sizeof(Direktorij));
    strcpy(root->naziv, "root");
    root->sljedeci = NULL;
    root->prethodni = NULL;
    root->roditelj = NULL;

    direktorij trenutni = root;

    unosKomande(&trenutni);

    oslobodiDirektorij(root);
    return 0;
}