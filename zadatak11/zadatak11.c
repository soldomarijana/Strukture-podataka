#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NAME 50
#define HASH_TABLE_SIZE 11
#define SUCCESS 0
#define FILE_NOT_OPENED (-1)
#define MEMORY_ERROR (-2)

struct _City;
typedef struct _City* CityPosition;

typedef struct _City
{
    char cityName[MAX_NAME];
    int population;
    CityPosition left;
    CityPosition right;
} City;


struct _Country;
typedef struct _Country* CountryPosition;

typedef struct _Country
{
    char countryName[MAX_NAME];
    CountryPosition next;
    CityPosition cityRoot;
} Country;


typedef struct _HashTable
{
    int size;
    CountryPosition* table;
} HashTable;

int initializeHashTable(HashTable* hashTable);
int readCountriesFromFile(HashTable* hashTable);
int calculateHashKey(char countryName[]);
int insertCountryIntoHash(HashTable* hashTable, char countryName[], char fileName[]);
int readCitiesFromFile(CityPosition* root, char fileName[]);
CityPosition insertCityIntoTree(CityPosition root, char cityName[], int population);
int printHashTable(HashTable* hashTable);
int printCityTree(CityPosition root);
int searchCountry(HashTable* hashTable, char countryName[], int populationLimit);
int searchCityTree(CityPosition root, int populationLimit);
int deleteCityTree(CityPosition root);
int deleteHashTable(HashTable* hashTable);


int main()
{
    HashTable hashTable = {0};
    char countryName[MAX_NAME] = {0};
    int populationLimit = 0;
    int result = 0;

    result = initializeHashTable(&hashTable);
    if (result != SUCCESS)
        return result;

    result = readCountriesFromFile(&hashTable);
    if (result != SUCCESS)
        return result;

    printHashTable(&hashTable);

    printf("\nUnesite ime drzave: ");
    scanf("%s", countryName);

    printf("Unesite minimalan broj stanovnika: ");
    scanf("%d", &populationLimit);

    result = searchCountry(&hashTable, countryName, populationLimit);
    if (result == 1)
        printf("Drzava nije pronadjena!\n");

    deleteHashTable(&hashTable);

    return SUCCESS;
}

int initializeHashTable(HashTable* hashTable)
{
    int i = 0;

    hashTable->size = HASH_TABLE_SIZE;
    hashTable->table = (CountryPosition*)malloc(sizeof(CountryPosition) * HASH_TABLE_SIZE);

    if (hashTable->table == NULL)
        return MEMORY_ERROR;

    for (i = 0; i < HASH_TABLE_SIZE; i++)
        hashTable->table[i] = NULL;

    return SUCCESS;
}

int calculateHashKey(char countryName[])
{
    int key = 0;
    int i = 0;

    for (i = 0; i < 5 && countryName[i] != '\0'; i++)
        key += (int)countryName[i];

    return key % HASH_TABLE_SIZE;
}

int readCountriesFromFile(HashTable* hashTable)
{
    FILE* file = fopen("drzave.txt", "r");
    char countryName[MAX_NAME] = {0};
    char fileName[MAX_NAME] = {0};

    if (file == NULL)
        return FILE_NOT_OPENED;

    while (fscanf(file, "%s %s", countryName, fileName) == 2)
        insertCountryIntoHash(hashTable, countryName, fileName);

    fclose(file);
    return SUCCESS;
}

int insertCountryIntoHash(HashTable* hashTable, char countryName[], char fileName[])
{
    int index = calculateHashKey(countryName);
    CountryPosition current = hashTable->table[index];
    CountryPosition previous = NULL;

    CountryPosition newCountry = (CountryPosition)malloc(sizeof(Country));
    if (newCountry == NULL)
        return MEMORY_ERROR;

    strcpy(newCountry->countryName, countryName);
    newCountry->cityRoot = NULL;
    newCountry->next = NULL;

    readCitiesFromFile(&newCountry->cityRoot, fileName);

    while (current != NULL && strcmp(current->countryName, countryName) < 0)
    {
        previous = current;
        current = current->next;
    }

    if (previous == NULL)
    {
        newCountry->next = hashTable->table[index];
        hashTable->table[index] = newCountry;
    }
    else
    {
        newCountry->next = current;
        previous->next = newCountry;
    }

    return SUCCESS;
}

int readCitiesFromFile(CityPosition* root, char fileName[])
{
    FILE* file = fopen(fileName, "r");
    char cityName[MAX_NAME] = { 0 };
    int population = 0;

    if (file == NULL)
        return FILE_NOT_OPENED;

    while (fscanf(file, " %[^,], %d", cityName, &population) == 2)
        *root = insertCityIntoTree(*root, cityName, population);

    fclose(file);
    return SUCCESS;
}

CityPosition insertCityIntoTree(CityPosition root, char cityName[], int population)
{
    if (root == NULL)
    {
        root = (CityPosition)malloc(sizeof(City));
        if (root == NULL)
            return NULL;

        strcpy(root->cityName, cityName);
        root->population = population;
        root->left = NULL;
        root->right = NULL;
    }
    else if (population < root->population ||
        (population == root->population && strcmp(cityName, root->cityName) < 0))
    {
        root->left = insertCityIntoTree(root->left, cityName, population);
    }
    else
    {
        root->right = insertCityIntoTree(root->right, cityName, population);
    }

    return root;
}

int printHashTable(HashTable* hashTable)
{
    int i = 0;
    CountryPosition current = NULL;

    for (i = 0; i < hashTable->size; i++)
    {
        printf("\n[%d]\n", i);
        current = hashTable->table[i];

        while (current != NULL)
        {
            printf("%s\n", current->countryName);
            printCityTree(current->cityRoot);
            current = current->next;
        }
    }
    return SUCCESS;
}

int printCityTree(CityPosition root)
{
    if (root == NULL)
        return SUCCESS;

    printCityTree(root->left);
    printf("  %s %d\n", root->cityName, root->population);
    printCityTree(root->right);

    return SUCCESS;
}

int searchCountry(HashTable* hashTable, char countryName[], int populationLimit)
{
    int index = calculateHashKey(countryName);
    CountryPosition current = hashTable->table[index];

    while (current != NULL)
    {
        if (strcmp(current->countryName, countryName) == 0)
        {
            searchCityTree(current->cityRoot, populationLimit);
            return SUCCESS;
        }
        current = current->next;
    }

    return 1;
}

int searchCityTree(CityPosition root, int populationLimit)
{
    if (root == NULL)
        return SUCCESS;

    if (root->population > populationLimit)
        printf("  %s %d\n", root->cityName, root->population);

    searchCityTree(root->left, populationLimit);
    searchCityTree(root->right, populationLimit);

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

int deleteHashTable(HashTable* hashTable)
{
    int i = 0;
    CountryPosition current = NULL;
    CountryPosition temp = NULL;

    for (i = 0; i < hashTable->size; i++)
    {
        current = hashTable->table[i];
        while (current != NULL)
        {
            temp = current;
            current = current->next;
            deleteCityTree(temp->cityRoot);
            free(temp);
        }
    }

    free(hashTable->table);
    return SUCCESS;
}