#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define RANDOM_MIN 10
#define RANDOM_MAX 90
#define NUM_COUNT 10
#define FILE_NOT_OPENED (-1)

typedef struct _Node* TreePosition;
typedef struct _Node {
    int value;
    TreePosition left;
    TreePosition right;
} Node;

TreePosition createNode(int value);
TreePosition insert(TreePosition root, int value);
int replace(TreePosition root);
void inorder(TreePosition root, FILE* file);
void deleteTree(TreePosition root);

int main() {
    TreePosition root = NULL;
    int numbers[NUM_COUNT] = {2, 5, 7, 8, 11, 1, 4, 2, 3, 7};
    int i;

    FILE* file = fopen("file.txt", "w");
    if (!file)
        return FILE_NOT_OPENED;

    for (i = 0; i < NUM_COUNT; i++)
        root = insert(root, numbers[i]);

    fprintf(file, "Inorder prije replace:\n");
    inorder(root, file);

    replace(root);

    fprintf(file, "\nInorder poslije replace:\n");
    inorder(root, file);

    deleteTree(root);
    root = NULL;

    srand((unsigned)time(NULL));

    fprintf(file, "\n\nRandom dio:\n");

    for (i = 0; i < NUM_COUNT; i++) {
        int x = rand() % (RANDOM_MAX - RANDOM_MIN + 1) + RANDOM_MIN;
        root = insert(root, x);
    }

    fprintf(file, "Random inorder prije replace:\n");
    inorder(root, file);

    replace(root);

    fprintf(file, "\nRandom inorder poslije replace:\n");
    inorder(root, file);

    deleteTree(root);
    fclose(file);

    return 0;
}

TreePosition createNode(int value) {
    TreePosition p = (TreePosition)malloc(sizeof(Node));
    if (!p)
        return NULL;

    p->value = value;
    p->left = NULL;
    p->right = NULL;

    return p;
}

TreePosition insert(TreePosition root, int value) {
    if (root == NULL)
        return createNode(value);

    if (value >= root->value)
        root->left = insert(root->left, value);
    else
        root->right = insert(root->right, value);

    return root;
}

int replace(TreePosition root) {
    if (root == NULL)
        return 0;

    int oldValue = root->value;

    root->value = replace(root->left) + replace(root->right);

    return root->value + oldValue;
}

void inorder(TreePosition root, FILE* file) {
    if (root == NULL)
        return;

    inorder(root->left, file);
    fprintf(file, "%d ", root->value);
    inorder(root->right, file);
}

void deleteTree(TreePosition root) {
    if (root == NULL)
        return;

    deleteTree(root->left);
    deleteTree(root->right);
    free(root);
}