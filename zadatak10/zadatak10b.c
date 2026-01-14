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

typedef struct _City
{
    char name[MAX_NAME];
    int population;
    CityPosition next;
} City;

struct _Country;
typedef struct _Country* CountryPosition;

typedef struct _Country
{
    char name[MAX_NAME];
    CountryPosition left;
    CountryPosition right;
    CityPosition cityHead;
} Country;

int readCountriesFromFile(CountryPosition* root);
CountryPosition insertCountryTree(CountryPosition root, char countryName[], char cityFileName[]);
int readCitiesFromFile(CityPosition* head, char fileName[]);
CityPosition insertCitySorted(CityPosition head, char cityName[], int population);
int printCountryTree(CountryPosition root);
int printCityList(CityPosition head);
int searchCountryTree(CountryPosition root, char countryName[], int populationLimit);
int deleteCityList(CityPosition head);
int deleteCountryTree(CountryPosition root);


int main(void)
{
    CountryPosition countryRoot = NULL;
    char countryName[MAX_NAME] = {0};
    int populationLimit = 0;
    int result = 0;

    result = readCountriesFromFile(&countryRoot);
    if (result != SUCCESS)
    {
        printf("Greska pri otvaranju datoteke!\n");
        return result;
    }

    printf("\nSVE DRZAVE I GRADOVI\n");
    printCountryTree(countryRoot);

    printf("\nUnesite ime drzave: ");
    scanf("%s", countryName);

    printf("Unesite minimalan broj stanovnika: ");
    scanf("%d", &populationLimit);

    result = searchCountryTree(countryRoot, countryName, populationLimit);
    if (result == 1)
        printf("Drzava nije pronadjena!\n");

    deleteCountryTree(countryRoot);
    return SUCCESS;
}

int readCountriesFromFile(CountryPosition* root)
{
    FILE* file = fopen("drzave.txt", "r");
    char countryName[MAX_NAME] = {0};
    char cityFileName[MAX_NAME] = {0};

    if (file == NULL)
        return FILE_NOT_OPENED;

    while (fscanf(file, "%s %s", countryName, cityFileName) == 2)
        *root = insertCountryTree(*root, countryName, cityFileName);

    fclose(file);
    return SUCCESS;
}

CountryPosition insertCountryTree(CountryPosition root, char countryName[], char cityFileName[])
{
    if (root == NULL)
    {
        root = (CountryPosition)malloc(sizeof(Country));
        if (root == NULL)
            return NULL;

        strcpy(root->name, countryName);
        root->left = NULL;
        root->right = NULL;
        root->cityHead = NULL;

        readCitiesFromFile(&root->cityHead, cityFileName);
    }
    else if (strcmp(countryName, root->name) < 0)
    {
        root->left = insertCountryTree(root->left, countryName, cityFileName);
    }
    else if (strcmp(countryName, root->name) > 0)
    {
        root->right = insertCountryTree(root->right, countryName, cityFileName);
    }

    return root;
}

int readCitiesFromFile(CityPosition* head, char fileName[])
{
    FILE* file = fopen(fileName, "r");
    char cityName[MAX_NAME] = { 0 };
    int population = 0;

    if (file == NULL)
        return FILE_NOT_OPENED;

    while (fscanf(file, " %[^,], %d", cityName, &population) == 2)
        *head = insertCitySorted(*head, cityName, population);

    fclose(file);
    return SUCCESS;
}

CityPosition insertCitySorted(CityPosition head, char cityName[], int population)
{
    CityPosition newCity = NULL;
    CityPosition current = head;

    newCity = (CityPosition)malloc(sizeof(City));
    if (newCity == NULL)
        return NULL;

    strcpy(newCity->name, cityName);
    newCity->population = population;
    newCity->next = NULL;

    if (head == NULL ||
        population < head->population ||
        (population == head->population && strcmp(cityName, head->name) < 0))
    {
        newCity->next = head;
        return newCity;
    }

    while (current->next != NULL &&
          (current->next->population < population ||
          (current->next->population == population &&
           strcmp(current->next->name, cityName) < 0)))
    {
        current = current->next;
    }

    newCity->next = current->next;
    current->next = newCity;

    return head;
}

int printCountryTree(CountryPosition root)
{
    if (root == NULL)
        return SUCCESS;

    printCountryTree(root->left);

    printf("\n%s\n", root->name);
    printCityList(root->cityHead);

    printCountryTree(root->right);
    return SUCCESS;
}

int printCityList(CityPosition head)
{
    while (head != NULL)
    {
        printf("  %s %d\n", head->name, head->population);
        head = head->next;
    }
    return SUCCESS;
}

int searchCountryTree(CountryPosition root, char countryName[], int populationLimit)
{
    if (root == NULL)
        return 1;

    if (strcmp(countryName, root->name) < 0)
        return searchCountryTree(root->left, countryName, populationLimit);
    else if (strcmp(countryName, root->name) > 0)
        return searchCountryTree(root->right, countryName, populationLimit);
    else
    {
        CityPosition city = root->cityHead;
        printf("\nGradovi u %s s brojem stanovnika vecim od %d:\n", root->name, populationLimit);

        while (city != NULL)
        {
            if (city->population > populationLimit)
                printf("  %s %d\n", city->name, city->population);
            city = city->next;
        }
        return SUCCESS;
    }
}

int deleteCityList(CityPosition head)
{
    CityPosition temp = NULL;

    while (head != NULL)
    {
        temp = head;
        head = head->next;
        free(temp);
    }
    return SUCCESS;
}

int deleteCountryTree(CountryPosition root)
{
    if (root == NULL)
        return SUCCESS;

    deleteCountryTree(root->left);
    deleteCountryTree(root->right);

    deleteCityList(root->cityHead);
    free(root);

    return SUCCESS;
}