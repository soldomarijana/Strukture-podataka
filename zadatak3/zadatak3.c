#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LENGTH 50
#define MEMORY_ERROR (-1)
#define FILE_ERROR (-2)

struct _Person;
typedef struct _Person* Position;
typedef struct _Person {
    char name[MAX_LENGTH];
    char surname[MAX_LENGTH];
    int birthYear;
    Position next;
} Person;

Position createPerson(char* name, char* surname, int birthYear);
int addToBeginning(Position head, char* name, char* surname, int birthYear);
int addToEnd(Position head, char* name, char* surname, int birthYear);
int printList(Position first);
Position findBySurname(Position first, char* surname);
int deleteBySurname(Position head, char* surname);
Position findLast(Position head);

int addAfter(Position head, char* targetSurname, char* name, char* surname, int birthYear);
int addBefore(Position head, char* targetSurname, char* name, char* surname, int birthYear);
int sortList(Position head);
int writeToFile(Position first, char* fileName);
int readFromFile(Position head, char* fileName);

int main() {
    Person head = { .name = "", .surname = "", .birthYear = 0, .next = NULL };
    char name[MAX_LENGTH], surname[MAX_LENGTH], targetSurname[MAX_LENGTH], fileName[MAX_LENGTH];
    int birthYear, choice;

    do {
        printf("\n--- IZBORNIK ---\n1 - Dodaj osobu na pocetak\n2 - Dodaj osobu na kraj\n3 - Ispisi listu\n4 - Pronadi osobu po prezimenu\n5 - Obrisi osobu po prezimenu\n6 - Dodaj osobu iza odredene osobe\n7 - Dodaj osobu ispred odredene osobe\n8 - Sortiraj listu po prezimenima\n9 - Spremi listu u datoteku\n10 - Ucitaj listu iz datoteke\n");
        printf("0 - Izlaz\n");
        printf("Izbor: ");
        scanf("%d", &choice);
        getchar();

        switch (choice) {
        case 1:
            printf("Unesi ime: "); fgets(name, MAX_LENGTH, stdin); name[strcspn(name, "\n")] = 0;
            printf("Unesi prezime: "); fgets(surname, MAX_LENGTH, stdin); surname[strcspn(surname, "\n")] = 0;
            printf("Unesi godinu rodjenja: "); scanf("%d", &birthYear);
            addToBeginning(&head, name, surname, birthYear);
            break;

        case 2:
            printf("Unesi ime: "); fgets(name, MAX_LENGTH, stdin); name[strcspn(name, "\n")] = 0;
            printf("Unesi prezime: "); fgets(surname, MAX_LENGTH, stdin); surname[strcspn(surname, "\n")] = 0;
            printf("Unesi godinu rodjenja: "); scanf("%d", &birthYear);
            addToEnd(&head, name, surname, birthYear);
            break;

        case 3:
            printList(head.next);
            break;

        case 4: {
            printf("Unesi prezime za pretragu: ");
            fgets(surname, MAX_LENGTH, stdin); surname[strcspn(surname, "\n")] = 0;
            Position found = findBySurname(head.next, surname);
            if (found)
                printf("Pronasli smo: %s %s (%d)\n", found->name, found->surname, found->birthYear);
            else
                printf("Osoba nije pronadena.\n");
            break;
        }

        case 5:
            printf("Unesi prezime za brisanje: ");
            fgets(surname, MAX_LENGTH, stdin); surname[strcspn(surname, "\n")] = 0;
            deleteBySurname(&head, surname);
            break;

        case 6:
            printf("Prezime osobe iza koje dodajes: ");
            fgets(targetSurname, MAX_LENGTH, stdin); targetSurname[strcspn(targetSurname, "\n")] = 0;
            printf("Ime nove osobe: "); fgets(name, MAX_LENGTH, stdin); name[strcspn(name, "\n")] = 0;
            printf("Prezime nove osobe: "); fgets(surname, MAX_LENGTH, stdin); surname[strcspn(surname, "\n")] = 0;
            printf("Godina rodjenja: "); scanf("%d", &birthYear);
            addAfter(&head, targetSurname, name, surname, birthYear);
            break;

        case 7:
            printf("Prezime osobe ispred koje dodajes: ");
            fgets(targetSurname, MAX_LENGTH, stdin); targetSurname[strcspn(targetSurname, "\n")] = 0;
            printf("Ime nove osobe: "); fgets(name, MAX_LENGTH, stdin); name[strcspn(name, "\n")] = 0;
            printf("Prezime nove osobe: "); fgets(surname, MAX_LENGTH, stdin); surname[strcspn(surname, "\n")] = 0;
            printf("Godina rodjenja: "); scanf("%d", &birthYear);
            addBefore(&head, targetSurname, name, surname, birthYear);
            break;

        case 8:
            sortList(&head);
            printf("Lista sortirana po prezimenima.\n");
            break;

        case 9:
            printf("Ime datoteke za spremanje: ");
            fgets(fileName, MAX_LENGTH, stdin); fileName[strcspn(fileName, "\n")] = 0;
            writeToFile(head.next, fileName);
            break;

        case 10:
            printf("Ime datoteke za ucitavanje: ");
            fgets(fileName, MAX_LENGTH, stdin); fileName[strcspn(fileName, "\n")] = 0;
            readFromFile(&head, fileName);
            break;

        case 0:
            printf("Izlaz iz programa.\n");
            break;

        default:
            printf("Nepoznat izbor.\n");
        }

    } while (choice != 0);

    Position temp = head.next;   //oslobadjanje memorije
    while (temp) {
        Position next = temp->next;
        free(temp);
        temp = next;
    }

    return 0;
}


Position createPerson(char* name, char* surname, int birthYear) {
    Position newPerson = (Position)malloc(sizeof(Person));
    if (!newPerson) return NULL;
    strcpy(newPerson->name, name);
    strcpy(newPerson->surname, surname);
    newPerson->birthYear = birthYear;
    newPerson->next = NULL;
    return newPerson;      //vraca pokazivac na novog clana
}

int addToBeginning(Position head, char* name, char* surname, int birthYear) {
    Position newPerson = createPerson(name, surname, birthYear);
    if (!newPerson) return MEMORY_ERROR;
    newPerson->next = head->next;
    head->next = newPerson;
    return 0;
}

Position findLast(Position head) {
    Position temp = head;
    while (temp->next)
        temp = temp->next;
    return temp;
}

int addToEnd(Position head, char* name, char* surname, int birthYear) {
    Position newPerson = createPerson(name, surname, birthYear);
    if (!newPerson) return MEMORY_ERROR;
    Position last = findLast(head);
    last->next = newPerson;
    return 0;
}

int printList(Position first) {
    if (!first) {
        printf("Lista je prazna.\n");
        return 0;
    }
    Position temp = first;
    while (temp) {
        printf("%s %s (%d)\n", temp->name, temp->surname, temp->birthYear);
        temp = temp->next;
    }
    return 0;
}

Position findBySurname(Position first, char* surname) {
    Position temp = first;
    while (temp) {
        if (strcmp(temp->surname, surname) == 0)
            return temp;        //ako prezime odgovara, vratit ce tu osobu
        temp = temp->next;
    }
    return NULL;
}

int deleteBySurname(Position head, char* surname) {
    Position prev = head;
    Position curr = head->next;
    while (curr && strcmp(curr->surname, surname) != 0) {
        prev = curr;
        curr = curr->next;
    }
    if (!curr) {
        printf("Osoba nije pronadjena.\n");
        return 1;
    }
    prev->next = curr->next;
    free(curr);
    printf("Osoba obrisana.\n");
    return 0;
}

int addAfter(Position head, char* targetSurname, char* name, char* surname, int birthYear) {
    Position target = findBySurname(head->next, targetSurname);
    if (!target) {
        printf("Osoba nije pronadjena.\n");
        return 1;
    }
    Position newPerson = createPerson(name, surname, birthYear);
    if (!newPerson) return MEMORY_ERROR;
    newPerson->next = target->next;
    target->next = newPerson;
    return 0;
}

int addBefore(Position head, char* targetSurname, char* name, char* surname, int birthYear) {
    Position prev = head;
    while (prev->next && strcmp(prev->next->surname, targetSurname) != 0)
        prev = prev->next;
    if (!prev->next) {
        printf("Osoba nije pronadjena.\n");
        return 1;
    }
    Position newPerson = createPerson(name, surname, birthYear);
    if (!newPerson) return MEMORY_ERROR;
    newPerson->next = prev->next;
    prev->next = newPerson;
    return 0;
}

int sortList(Position head) {            //sortiranje liste po prezimenima pomocu bubble sorta
    if (!head->next) return 0;
    int swapped;
    Position prev, curr, end = NULL;
    do {
        swapped = 0;                    //zastavica za provjeru ako je doslo do zamjene
        prev = head;
        curr = head->next;
        while (curr->next != end) {
            if (strcmp(curr->surname, curr->next->surname) > 0) {
                Position tmp = curr->next;
                curr->next = tmp->next;
                tmp->next = curr;
                prev->next = tmp;
                swapped = 1;
            }
            prev = prev->next;
            curr = prev->next;
        }
        end = curr;       //kraj je vec sortiran 
    } while (swapped);
    return 0;
}

int writeToFile(Position first, char* fileName) {
    FILE* f = fopen(fileName, "w");
    if (!f) {
        printf("Greska pri otvaranju datoteke.\n");
        return FILE_ERROR;
    }
    Position temp = first;
    while (temp) {
        fprintf(f, "%s %s %d\n", temp->name, temp->surname, temp->birthYear);
        temp = temp->next;
    }
    fclose(f);
    printf("Lista spremljena u datoteku.\n");
    return 0;
}

int readFromFile(Position head, char* fileName) {
    FILE* f = fopen(fileName, "r");
    if (!f) {
        printf("Greska pri otvaranju datoteke.\n");
        return FILE_ERROR;
    }

    
    Position temp = head->next;
    while (temp) {                     //brise se stara lista prije ucitavanja nove
        Position next = temp->next;
        free(temp);
        temp = next;
    }
    head->next = NULL;

    char name[MAX_LENGTH], surname[MAX_LENGTH];
    int birthYear;

    while (fscanf(f, "%s %s %d", name, surname, &birthYear) == 3)
        addToEnd(head, name, surname, birthYear);

    fclose(f);
    printf("Lista ucitana iz datoteke.\n");
    return 0;
}