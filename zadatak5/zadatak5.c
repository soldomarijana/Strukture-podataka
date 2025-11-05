#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINE 1024
#define MEMORY_ERROR (-1)
#define FILE_ERROR (-2)
#define FILENAME "postfix.txt"

struct _Postfix;
typedef struct _Postfix* Position;
typedef struct _Postfix {
    double number;
    Position next;
} Postfix;

int push(Position head, double number);
double pop(Position head);
int calculatePostfixFromFile(char* fileName, Position head);
int freeAll(Position head);

int main() {
    Postfix head = { .number = 0, .next = NULL };
    int status = EXIT_SUCCESS;

    status = calculatePostfixFromFile(FILENAME, &head);

    freeAll(&head);

    return status;
}

int push(Position head, double number) {   
    Position newEl = (Position)malloc(sizeof(Postfix));
    if (newEl == NULL) {
        printf("Greska: neuspjesna alokacija memorije!\n");
        return MEMORY_ERROR;
    }

    newEl->number = number;
    newEl->next = head->next;
    head->next = newEl;

    return EXIT_SUCCESS;
}

double pop(Position head) { 
    if (head->next == NULL) {
        printf("Greska: stog je prazan!\n");
        return 0;
    }

    Position temp = head->next;
    double number = temp->number;

    head->next = temp->next;
    temp->next = NULL;
    free(temp);

    return number;
}

int calculatePostfixFromFile(char* fileName, Position head) {
    FILE* file = fopen(fileName, "r");
    if (file == NULL) {
        printf("Greska: datoteka '%s' se ne moze otvoriti!\n", fileName);
        return FILE_ERROR;
    }

    char buffer[MAX_LINE] = {0};
    char* token = NULL;
    double num = 0.0, operand1 = 0.0, operand2 = 0.0, result = 0.0;
    char operation = 0;

    if (fgets(buffer, MAX_LINE, file) == NULL) {    
        printf("Datoteka je prazna!\n");
        fclose(file);
        return EXIT_FAILURE;
    }

    token = strtok(buffer, " \n\t");

    while (token != NULL) {
        if (sscanf(token, "%lf", &num) == 1) {
            if (push(head, num) != EXIT_SUCCESS) {
                fclose(file);
                return EXIT_FAILURE;
            }
        }
        else if (sscanf(token, " %c", &operation) == 1) {
            operand2 = pop(head);
            operand1 = pop(head);

            switch (operation) {
            case '+':
                result = operand1 + operand2;
                break;
            case '-':
                result = operand1 - operand2;
                break;
            case '*':
                result = operand1 * operand2;
                break;
            case '/':
                if (operand2 == 0) {
                    printf("Greska: dijeljenje s nulom!\n");
                    fclose(file);
                    return EXIT_FAILURE;
                }
                result = operand1 / operand2;
                break;
            default:
                printf("Nepoznat operator '%c'\n", operation);
                fclose(file);
                return EXIT_FAILURE;
            }

            push(head, result);
        }
        else {
            printf("Neispravan unos: %s\n", token);
            fclose(file);
            return EXIT_FAILURE;
        }

        token = strtok(NULL, " \n\t");
    }

    if (head->next != NULL && head->next->next == NULL)
        printf("Rezultat postfix izraza je: %.2lf\n", head->next->number);
    else
        printf("Greska: postfix izraz nije ispravan.\n");

    fclose(file);
    return EXIT_SUCCESS;
}

int freeAll(Position head) {   
    Position temp = NULL;
    while (head->next != NULL) {
        temp = head->next;
        head->next = temp->next;
        temp->next = NULL;
        free(temp);
    }
    return EXIT_SUCCESS;
}
