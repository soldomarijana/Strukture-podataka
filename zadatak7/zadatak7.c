#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NAME 64
#define SUCCESS 0              
#define MEMORY_ERROR (-1)       
#define DIR_NOT_FOUND 1        
#define ROOT_POP_ATTEMPT 2      

typedef struct _Directory* DirPosition;
typedef struct _Directory
{
    char name[MAX_NAME];     
    DirPosition next;        
    DirPosition subDir;   
} Directory;


typedef struct _Stack* StackPosition;
typedef struct _Stack
{
    DirPosition dirLevel; 
    StackPosition next;
} Stack;


int push(StackPosition head, DirPosition dir);
int pop(StackPosition head);

DirPosition createDirectory(const char* name);
int makeDirectory(StackPosition stackHead, const char* name);
int changeDirectory(StackPosition stackHead, const char* name);

int printDirectoryTree(DirPosition current, int level);
int freeDirectoryTree(DirPosition current);
int freeStack(StackPosition head);


int main()
{
    DirPosition root = NULL;
    Stack stackHead;
    char dirName[MAX_NAME] = {0};
    int choice = 0;
    int result = 0;

    stackHead.next = NULL;

    root = createDirectory("C");
    if (!root)
    {
        printf("Greska: nije moguce alocirati memoriju za root direktorij.\n");
        return MEMORY_ERROR;
    }

    result = push(&stackHead, root);
    if (result != SUCCESS)
    {
        printf("Greska: neuspjela inicijalizacija stacka.\n");
        freeDirectoryTree(root);
        return result;
    }

    while (1)
    {
        printf("\n--- IZBORNIK ---\n");
        printf("1 - md (kreiraj direktorij)\n");
        printf("2 - cd dir (ulaz u direktorij)\n");
        printf("3 - cd .. (povratak u prethodni direktorij)\n");
        printf("4 - dir (ispis direktorija)\n");
        printf("5 - izlaz\n");
        printf("Odabir: ");

        if (scanf("%d", &choice) != 1)
        {
            printf("Neispravan unos. Molim unesite broj od 1 do 5.\n");
            while (getchar() != '\n');
            continue;
        }

        switch (choice)
        {
        case 1:
            printf("Unesite naziv direktorija: ");
            scanf("%63s", dirName);

            result = makeDirectory(&stackHead, dirName);
            if (result == MEMORY_ERROR)
                printf("Greska: nije moguce alocirati memoriju za novi direktorij.\n");
            else
                printf("Direktorij je uspjesno kreiran.\n");
            break;

        case 2:
            printf("Unesite naziv direktorija u koji zelite uci: ");
            scanf("%63s", dirName);

            result = changeDirectory(&stackHead, dirName);
            if (result == DIR_NOT_FOUND)
                printf("Greska: trazeni direktorij ne postoji.\n");
            else if (result == MEMORY_ERROR)
                printf("Greska: problem s memorijom prilikom promjene direktorija.\n");
            else
                printf("Uspjesno ste usli u direktorij.\n");
            break;

        case 3:
            result = pop(&stackHead);
            if (result == ROOT_POP_ATTEMPT)
                printf("Vec se nalazite u root direktoriju. Povratak nije moguc.\n");
            else
                printf("Uspjesno ste se vratili u prethodni direktorij.\n");
            break;

        case 4:
            printf("\nStruktura direktorija:\n");
            printDirectoryTree(root, 0);
            break;

        case 5:
            freeStack(&stackHead);
            freeDirectoryTree(root);
            printf("Program je zavrsen i sva memorija je oslobodjena.\n");
            return SUCCESS;

        default:
            printf("Neispravan odabir. Molimo pokusajte ponovno.\n");
        }
    }
}

int push(StackPosition head, DirPosition dir)  //dodaje direktorij na stack
{
    StackPosition newElement = (StackPosition)malloc(sizeof(Stack));
    if (!newElement)
        return MEMORY_ERROR;

    newElement->dirLevel = dir;
    newElement->next = head->next;
    head->next = newElement;

    return SUCCESS;
}

int pop(StackPosition head)  //uklanja vrh stacka (cd ..), ali ne dopušta izlazak iz root direktorija
{
    if (head->next == NULL)
        return SUCCESS;

    if (head->next->next == NULL)
        return ROOT_POP_ATTEMPT;

    StackPosition temp = head->next;
    head->next = temp->next;
    free(temp);

    return SUCCESS;
}

DirPosition createDirectory(const char* name)
{
    DirPosition newDir = (DirPosition)malloc(sizeof(Directory));
    if (!newDir)
        return NULL;

    strcpy(newDir->name, name);
    newDir->next = NULL;
    newDir->subDir = NULL;

    return newDir;
}

int makeDirectory(StackPosition stackHead, const char* name)   //md–kreira poddirektorij u trenutnom direktoriju 
{
    DirPosition parent = stackHead->next->dirLevel;
    DirPosition newDir = createDirectory(name);

    if (!newDir)
        return MEMORY_ERROR;

    newDir->next = parent->subDir;
    parent->subDir = newDir;

    return SUCCESS;
}

int changeDirectory(StackPosition stackHead, const char* name)  //cd dir-ulazi u direktorij ako postoji 
{
    DirPosition current = stackHead->next->dirLevel->subDir;

    while (current)
    {
        if (strcmp(current->name, name) == 0)
            return push(stackHead, current);

        current = current->next;
    }

    return DIR_NOT_FOUND;
}

int printDirectoryTree(DirPosition current, int level)  //rekurzivni ispis stabla direktorija
{
    while (current)
    {
        for (int i = 0; i < level; i++)
            printf("  ");

        printf("%s\n", current->name);

        if (current->subDir)
            printDirectoryTree(current->subDir, level + 1);

        current = current->next;
    }
    return SUCCESS;
}

int freeDirectoryTree(DirPosition current) 
{
    if (!current)
        return SUCCESS;

    freeDirectoryTree(current->subDir);
    freeDirectoryTree(current->next);
    free(current);

    return SUCCESS;
}

int freeStack(StackPosition head) 
{
    while (head->next)
    {
        StackPosition temp = head->next;
        head->next = temp->next;
        free(temp);
    }
    return SUCCESS;
}
