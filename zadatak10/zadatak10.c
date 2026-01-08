#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NAME 50
#define SUCCESS 0
#define FILE_NOT_OPENED (-1)
#define MEMORY_ERROR (-2)

struct _City;
typedef struct _City* CityPosition;

typedef struct _City {
    char name[MAX_NAME];
    int population;
    CityPosition left;
    CityPosition right;
} City;

struct _Country;
typedef struct _Country* CountryPosition;

typedef struct _Country {
    char name[MAX_NAME];
    CountryPosition next;
    CityPosition root;
} Country;

int readCountriesFromFile(CountryPosition head);
CountryPosition insertCountrySorted(CountryPosition head, char countryName[], char fileName[]);
int readCitiesFromFile(CityPosition* root, char fileName[]);
CityPosition insertCity(CityPosition root, char cityName[], int population);

int printCountries(CountryPosition head);
int printCityTree(CityPosition root);

int searchCityTree(CityPosition root, int limit);

int deleteCityTree(CityPosition root);
int deleteCountryList(CountryPosition head);

int main()
{
    Country head = {.name = "", .next = NULL, .root = NULL};
    char countryName[MAX_NAME] = {0};
    int limit = 0;

    if (readCountriesFromFile(&head) != SUCCESS)
    {
        printf("Greska pri otvaranju datoteke!\n");
        return FILE_NOT_OPENED;
    }

    printf("SVE DRZAVE I GRADOVI\n");
    printCountries(head.next);

    printf("\nUnesite ime drzave: ");
    scanf("%s", countryName);

    printf("Unesite minimalan broj stanovnika: ");
    scanf("%d", &limit);

    CountryPosition current = head.next;
    while (current != NULL && strcmp(current->name, countryName) != 0)
        current = current->next;

    if (current == NULL)
    {
        printf("Drzava nije pronadjena!\n");
    }
    else
    {
        printf("\nGradovi u %s s brojem stanovnika vecim od %d:\n", current->name, limit);
        searchCityTree(current->root, limit);
    }

    deleteCountryList(head.next);
    return SUCCESS;
}

int readCountriesFromFile(CountryPosition head)
{
    FILE* file = fopen("drzave.txt", "r");
    char countryName[MAX_NAME] = {0};
    char fileName[MAX_NAME] = {0};

    if (file == NULL)
        return FILE_NOT_OPENED;

    while (fscanf(file, "%s %s", countryName, fileName) == 2)
        insertCountrySorted(head, countryName, fileName);

    fclose(file);
    return SUCCESS;
}

CountryPosition insertCountrySorted(CountryPosition head, char countryName[], char fileName[])
{
    CountryPosition p = head;

    while (p->next != NULL && strcmp(p->next->name, countryName) < 0)
        p = p->next;

    CountryPosition newCountry = (CountryPosition)malloc(sizeof(Country));
    if (newCountry == NULL)
        return NULL;

    strcpy(newCountry->name, countryName);
    newCountry->root = NULL;

    readCitiesFromFile(&newCountry->root, fileName);

    newCountry->next = p->next;
    p->next = newCountry;

    return newCountry;
}

int readCitiesFromFile(CityPosition* root, char fileName[])
{
    FILE* file = fopen(fileName, "r");
    char cityName[MAX_NAME] = {0};
    int population = 0;

    if (file == NULL)
        return FILE_NOT_OPENED;

    while (fscanf(file, " %[^,], %d", cityName, &population) == 2)
        *root = insertCity(*root, cityName, population);

    fclose(file);
    return SUCCESS;
}

CityPosition insertCity(CityPosition root, char cityName[], int population)
{
    if (root == NULL)
    {
        root = (CityPosition)malloc(sizeof(City));
        if (root == NULL)
            return NULL;

        strcpy(root->name, cityName);
        root->population = population;
        root->left = NULL;
        root->right = NULL;
    }
    else if (population < root->population ||
            (population == root->population &&
             strcmp(cityName, root->name) < 0))
    {
        root->left = insertCity(root->left, cityName, population);
    }
    else
    {
        root->right = insertCity(root->right, cityName, population);
    }

    return root;
}

int printCountries(CountryPosition head)
{
    while (head != NULL)
    {
        printf("\n%s\n", head->name);
        printCityTree(head->root);
        head = head->next;
    }
    return SUCCESS;
}

int printCityTree(CityPosition root)
{
    if (root == NULL)
        return SUCCESS;

    printCityTree(root->left);
    printf("  %s %d\n", root->name, root->population);
    printCityTree(root->right);

    return SUCCESS;
}

int searchCityTree(CityPosition root, int limit)
{
    if (root == NULL)
        return SUCCESS;

    if (root->population > limit)
        printf("  %s %d\n", root->name, root->population);

    searchCityTree(root->left, limit);
    searchCityTree(root->right, limit);

    return SUCCESS;
}

int deleteCityTree(CityPosition root)
{
    if (root == NULL)
        return SUCCESS;

    deleteCityTree(root->left);
    deleteCityTree(root->right);
    free(root);

    return SUCCESS;
}

int deleteCountryList(CountryPosition head)
{
    CountryPosition temp = NULL;

    while (head != NULL)
    {
        temp = head;
        head = head->next;
        deleteCityTree(temp->root);
        free(temp);
    }
    return SUCCESS;
}