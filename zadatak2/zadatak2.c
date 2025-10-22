#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LENGTH 50
#define MEMORY_ERROR (-1)

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

int main() {
    Person head = { .name = "", .surname = "", .birthYear = 0, .next = NULL };
    char name[MAX_LENGTH], surname[MAX_LENGTH];
    int birthYear;
    int choice;

    do {
        printf("\n--- IZBORNIK ---\n1 - Dodaj osobu na pocetak\n2 - Dodaj osobu na kraj\n3 - Ispisi listu\n4 - Pronadi osobu po prezimenu\n5 - Obrisi osobu po prezimenu\n");
        printf("0 - Izlaz\n");
        printf("Izbor: ");
        scanf(" %d", &choice);
        getchar(); 

        switch (choice) {
        case 1:
            printf("Unesi ime: ");
            fgets(name, MAX_LENGTH, stdin);
            name[strcspn(name, "\n")] = 0; 
            printf("Unesi prezime: ");
            fgets(surname, MAX_LENGTH, stdin);
            surname[strcspn(surname, "\n")] = 0;
            printf("Unesi godinu rodjenja: ");
            scanf("%d", &birthYear);
            addToBeginning(&head, name, surname, birthYear);
            break;
        case 2:
            printf("Unesi ime: ");
            fgets(name, MAX_LENGTH, stdin);
            name[strcspn(name, "\n")] = 0;
            printf("Unesi prezime: ");
            fgets(surname, MAX_LENGTH, stdin);
            surname[strcspn(surname, "\n")] = 0;
            printf("Unesi godinu rodjenja: ");
            scanf("%d", &birthYear);
            addToEnd(&head, name, surname, birthYear);
            break;
        case 3:
            printList(head.next);
            break;
        case 4:
            printf("Unesi prezime za pretragu: ");
            fgets(surname, MAX_LENGTH, stdin);
            surname[strcspn(surname, "\n")] = 0;
            Position found = findBySurname(head.next, surname);
            if (found)
                printf("Pronasli smo: %s %s (%d)\n", found->name, found->surname, found->birthYear);
            else
                printf("Osoba nije pronadena!\n");
            break;
        case 5:
            printf("Unesi prezime za brisanje: ");
            fgets(surname, MAX_LENGTH, stdin);
            surname[strcspn(surname, "\n")] = 0;
            deleteBySurname(&head, surname);
            getchar();
            break;
        case 0:
            printf("Izlaz iz programa.\n");
            break;
        default:
            printf("Nepoznat izbor!\n");
        }

    } while (choice != 0);

    Position temp = head.next;
    while (temp != NULL) {
        Position next = temp->next;
        free(temp);
        temp = next;
    }

    return 0;
}

Position createPerson(char* name, char* surname, int birthYear) {
    Position newPerson = (Position)malloc(sizeof(Person));
    if (newPerson == NULL) {
        printf("Greska: memorija nije alocirana.\n");
        return NULL;
    }
    strcpy(newPerson->name, name);
    strcpy(newPerson->surname, surname);
    newPerson->birthYear = birthYear;
    newPerson->next = NULL;
    return newPerson;
}

int addToBeginning(Position head, char* name, char* surname, int birthYear) {
    Position newPerson = createPerson(name, surname, birthYear);
    if (newPerson == NULL)
        return MEMORY_ERROR;

    newPerson->next = head->next;
    head->next = newPerson;

    printf("Dodan");
    getchar();
    return 0;
}

Position findLast(Position head) {
    Position temp = head;
    while (temp->next != NULL)
        temp = temp->next;
    return temp;
}

int addToEnd(Position head, char* name, char* surname, int birthYear) {
    Position newPerson = createPerson(name, surname, birthYear);
    if (newPerson == NULL)
        return MEMORY_ERROR;

    Position last = findLast(head);
    last->next = newPerson;

    printf("Dodan");

    return 0;
}

int printList(Position first) {
    if (first == NULL) {
        printf("Lista je prazna.\n");
        return 0;
    }

    Position temp = first;
    while (temp != NULL) {
        printf("%s %s (%d)\n", temp->name, temp->surname, temp->birthYear);
        temp = temp->next;
    }

    return 0;
}

Position findBySurname(Position first, char* surname) {
    Position temp = first;
    while (temp != NULL) {
        if (strcmp(temp->surname, surname) == 0)
            return temp;
        temp = temp->next;
    }
    return NULL;
}

int deleteBySurname(Position head, char* surname) {
    Position prev = head;
    Position curr = head->next;

    while (curr != NULL && strcmp(curr->surname, surname) != 0) {
        prev = curr;
        curr = curr->next;
    }

    if (curr == NULL) {
        printf("Osoba s prezimenom %s nije pronadjena.\n", surname);
        return 1; 
    }

    prev->next = curr->next;
    free(curr);

    printf("Osoba %s uspjesno obrisana.\n", surname);
    return 0;
}

