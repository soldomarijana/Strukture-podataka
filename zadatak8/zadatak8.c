#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

#define SUCCESS 0
#define QUEUE_SIZE 100
#define MEMORY_ERROR (-1)

typedef struct _tree* TreePosition;

typedef struct _tree {
    int value;
    TreePosition left;
    TreePosition right;
} Tree;

TreePosition insertTree(TreePosition root, int value);
TreePosition findElement(TreePosition root, int value);
TreePosition deleteTree(TreePosition root, int value);
TreePosition findMin(TreePosition root);

int inorder(TreePosition root);
int preorder(TreePosition root);
int postorder(TreePosition root);
int levelOrder(TreePosition root);

int freeTree(TreePosition root);

int main()
{
    TreePosition root = NULL;
    int choice = 0;
    int value = 0;

    do {
        printf("\n--- BINARNO STABLO PRETRAZIVANJA ---\n");
        printf("1 - Unos elementa\n");
        printf("2 - Inorder ispis\n");
        printf("3 - Preorder ispis\n");
        printf("4 - Postorder ispis\n");
        printf("5 - Level order ispis\n");
        printf("6 - Pronadi element\n");
        printf("7 - Obrisi element\n");
        printf("0 - Izlaz\n");
        printf("Odabir: ");
        scanf("%d", &choice);

        switch (choice) {
        case 1:
            printf("Unesi vrijednost: ");
            scanf("%d", &value);
            root = insertTree(root, value);
            break;

        case 2:
            inorder(root);
            printf("\n");
            break;

        case 3:
            preorder(root);
            printf("\n");
            break;

        case 4:
            postorder(root);
            printf("\n");
            break;

        case 5:
            levelOrder(root);
            printf("\n");
            break;

        case 6:
            printf("Unesi vrijednost: ");
            scanf("%d", &value);
            if (findElement(root, value) != NULL)
                printf("Element postoji u stablu.\n");
            else
                printf("Element ne postoji.\n");
            break;

        case 7:
            printf("Unesi vrijednost: ");
            scanf("%d", &value);
            root = deleteTree(root, value);
            break;
        }
    } while (choice != 0);

    freeTree(root);
    return SUCCESS;
}

TreePosition insertTree(TreePosition root, int value)
{
    if (root == NULL) {
        root = (TreePosition)malloc(sizeof(Tree));
        if (!root)
            return NULL;

        root->value = value;
        root->left = NULL;
        root->right = NULL;
    }
    else if (value < root->value) {
        root->left = insertTree(root->left, value);
    }
    else if (value > root->value) {
        root->right = insertTree(root->right, value);
    }

    return root;
}

TreePosition findElement(TreePosition root, int value)
{
    if (root == NULL)
        return NULL;

    if (value < root->value)
        return findElement(root->left, value);
    else if (value > root->value)
        return findElement(root->right, value);
    else
        return root;
}

int inorder(TreePosition root)
{
    if (root != NULL) {
        inorder(root->left);
        printf("%d ", root->value);
        inorder(root->right);
    }
    return SUCCESS;
}

int preorder(TreePosition root)
{
    if (root != NULL) {
        printf("%d ", root->value);
        preorder(root->left);
        preorder(root->right);
    }
    return SUCCESS;
}

int postorder(TreePosition root)
{
    if (root != NULL) {
        postorder(root->left);
        postorder(root->right);
        printf("%d ", root->value);
    }
    return SUCCESS;
}

int levelOrder(TreePosition root)
{
    if (root == NULL)
        return SUCCESS;

    TreePosition queue[QUEUE_SIZE];
    int front = 0, rear = 0;

    queue[rear++] = root;

    while (front < rear) {
        TreePosition current = queue[front++];
        printf("%d ", current->value);

        if (current->left != NULL)
            queue[rear++] = current->left;
        if (current->right != NULL)
            queue[rear++] = current->right;
    }
    return SUCCESS;
}

TreePosition findMin(TreePosition root)
{
    while (root->left != NULL)
        root = root->left;

    return root;
}

TreePosition deleteTree(TreePosition root, int value)
{
    if (root == NULL)
        return NULL;

    if (value < root->value) {
        root->left = deleteTree(root->left, value);
    }
    else if (value > root->value) {
        root->right = deleteTree(root->right, value);
    }
    else {
        if (root->left != NULL && root->right != NULL) {
            TreePosition temp = findMin(root->right);
            root->value = temp->value;
            root->right = deleteTree(root->right, temp->value);
        }
        else {
            TreePosition temp = root;
            if (root->left == NULL)
                root = root->right;
            else
                root = root->left;

            free(temp);
        }
    }
    return root;
}

int freeTree(TreePosition root)
{
    if (root != NULL) {
        freeTree(root->left);
        freeTree(root->right);
        free(root);
    }
    return SUCCESS;
}
