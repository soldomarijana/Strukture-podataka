#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE (1024)
#define FILE_NOT_OPENED (-1)
#define MEMORY_NOT_ALLOCATED (-2)


typedef struct _Poly* Position;
typedef struct _Poly {
    int coeff;
    int pow;
    Position next;
} Poly;


int readPolynomialFromFile(char* fileName, Position head);
int insertSorted(Position head, int coeff, int pow);
int printPolynomial(Position head);
int addPolynomials(Position p1, Position p2, Position result);
int multiplyPolynomials(Position p1, Position p2, Position result);
int deleteAll(Position head);
Position createElement(int coeff, int pow);


int main(void) {

    Poly firstHead = { .coeff = 0, .pow = 0, .next = NULL };
    Poly secondHead = { .coeff = 0, .pow = 0, .next = NULL };
    Poly sumHead = { .coeff = 0, .pow = 0, .next = NULL };
    Poly productHead = { .coeff = 0, .pow = 0, .next = NULL };

    int status = 0;

    status = readPolynomialFromFile("p1.txt", &firstHead);
    if (status < 0) return EXIT_FAILURE;

    status = readPolynomialFromFile("p2.txt", &secondHead);
    if (status < 0) return EXIT_FAILURE;

    printf("First polynomial:\n");
    printPolynomial(firstHead.next);
    printf("\nSecond polynomial:\n");
    printPolynomial(secondHead.next);

    addPolynomials(firstHead.next, secondHead.next, &sumHead);
    printf("\nSum of polynomials:\n");
    printPolynomial(sumHead.next);

    multiplyPolynomials(firstHead.next, secondHead.next, &productHead);
    printf("\nProduct of polynomials:\n");
    printPolynomial(productHead.next);

    deleteAll(&firstHead);
    deleteAll(&secondHead);
    deleteAll(&sumHead);
    deleteAll(&productHead);

    return EXIT_SUCCESS;
}

Position createElement(int coeff, int pow) {          
    Position newEl = (Position)malloc(sizeof(Poly));
    if (!newEl) {
        printf("Greska: neuspjesna alokacija memorije!\n");
        return NULL;
    }

    newEl->coeff = coeff;
    newEl->pow = pow;
    newEl->next = NULL;
    return newEl;
}

int readPolynomialFromFile(char* fileName, Position head) {
    FILE* filePointer = fopen(fileName, "r");
    if (!filePointer) {
        printf("Greska: Datoteka %s se ne može otvoriti.\n", fileName);
        return FILE_NOT_OPENED;
    }

    int coeff = 0;
    int pow = 0;

    while (fscanf(filePointer, "%d %d", &coeff, &pow) == 2) {
        insertSorted(head, coeff, pow);
    }

    fclose(filePointer);
    return 0;
}

//umeće se novi član u listu da ostane sortirana po eksponentu
int insertSorted(Position head, int coeff, int pow) { 
    Position newEl = createElement(coeff, pow);
    if (!newEl) return MEMORY_NOT_ALLOCATED;

    Position current = head;

    while (current->next != NULL && current->next->pow > pow)     //tražimo gdje trebamo ubaciti novi element
        current = current->next;

    if (current->next != NULL && current->next->pow == pow) {        //ako već postoji isti eksponent, zbrojimo koeficijente
        current->next->coeff += coeff;

        if (current->next->coeff == 0) {             //ako nakon zbrajanja koeficijent postane 0, brišemo taj član
            Position toDelete = current->next;
            current->next = toDelete->next;
            free(toDelete);
        }
        free(newEl);
        return 0;
    }

    newEl->next = current->next;
    current->next = newEl;

    return 0;
}

int printPolynomial(Position head) {
    Position current = head;

    if (!current) {
        printf("Polinom je prazan.\n");
        return 0;
    }

    while (current != NULL) {
        printf("%dx^%d", current->coeff, current->pow);
        if (current->next != NULL && current->next->coeff >= 0)
            printf(" + ");
        else if (current->next != NULL)
            printf(" ");
        current = current->next;
    }
    printf("\n");
    return 0;
}

int addPolynomials(Position p1, Position p2, Position result) {
    //dodaje sve članove prvog polinoma
    while (p1 != NULL) {
        insertSorted(result, p1->coeff, p1->pow);
        p1 = p1->next;
    }
    //dodaje sve članove drugog polinoma
    while (p2 != NULL) {
        insertSorted(result, p2->coeff, p2->pow);
        p2 = p2->next;
    }

    return 0;
}

int multiplyPolynomials(Position p1, Position p2, Position result) { 
    for (Position i = p1; i != NULL; i = i->next) {
        for (Position j = p2; j != NULL; j = j->next) {
            //pri množenju članova polinoma koeficijenti se množe, a eksponenti zbrajaju
            int newCoeff = i->coeff * j->coeff;       
            int newPow = i->pow + j->pow;
            //svaki rezultat ubacujemo sortirano, ako se ponovi isti eksponent zbrajat će se
            insertSorted(result, newCoeff, newPow);    
        }
    }
    return 0;
}

int deleteAll(Position head) {
    Position temp = NULL;

    while (head->next != NULL) {
        temp = head->next;
        head->next = temp->next;
        free(temp);
    }
    return 0;
}