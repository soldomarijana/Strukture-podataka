#include <stdio.h>
#include <stdlib.h>

#define MAX_BODOVI 100.0
#define BUFFER_SIZE 1024
#define ERROR_OF -1
#define FILENAME "studenti.txt"

typedef struct {
    char ime[15];
    char prezime[30];
    int bodovi;
} Student;

int count_rows(const char *filename);
Student* read_students(const char *filename, int broj_studenata);
void print_students(Student *studenti, int broj_studenata);


int main() {
    int broj_studenata = count_rows(FILENAME);
    if (broj_studenata == ERROR_OF) return ERROR_OF;

    Student *studenti = read_students(FILENAME, broj_studenata);
    if (studenti == NULL) return ERROR_OF;

    print_students(studenti, broj_studenata);
    free(studenti);

    return 0;
}

int count_rows(const char *filename) {
    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("Greška: neuspješno otvaranje datoteke.\n");
        return ERROR_OF;
    }

    char buffer[BUFFER_SIZE];
    int count = 0;

    while (fgets(buffer, BUFFER_SIZE, fp) != NULL)
        count++;

    fclose(fp);
    return count;
}

Student* read_students(const char *filename, int broj_studenata) {
    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("Greška: neuspješno otvaranje datoteke.\n");
        return NULL;
    }

    Student *studenti = (Student*)malloc(broj_studenata * sizeof(Student));
    if (studenti == NULL) {
        printf("Greška: alokacija memorije nije uspjela.\n");
        fclose(fp);
        return NULL;
    }

    for (int i = 0; i < broj_studenata; i++) {
        if (fscanf(fp, "%s %s %d", studenti[i].ime, studenti[i].prezime, &studenti[i].bodovi) != 3) {
            printf("Greška: neispravan format datoteke u retku %d.\n", i + 1);
            free(studenti);
            fclose(fp);
            return NULL;
        }
    }

    fclose(fp);
    return studenti;
}

void print_students(Student *studenti, int broj_studenata) {
    printf("\n%-15s %-20s %-10s %-20s\n", "Ime", "Prezime", "Bodovi", "Relativni bodovi (%)");

    for (int i = 0; i < broj_studenata; i++) {
        double relativni = (studenti[i].bodovi / MAX_BODOVI) * 100.0;
        printf("%-15s %-20s %-10d %10.2f%%\n",
               studenti[i].ime, studenti[i].prezime, studenti[i].bodovi, relativni);
    }
}