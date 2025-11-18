#include "tree.h"
#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    node_t *root = NULL;

    root = tree_insert(root, 50);
    root = tree_insert(root, 30);
    root = tree_insert(root, 20);
    root = tree_insert(root, 40);
    root = tree_insert(root, 70);
    root = tree_insert(root, 60);
    root = tree_insert(root, 80);

    printf("[1] The tree: ");
    tree_print(root);
    printf("\n");

    root = tree_insert(root, 45);

    printf("[2] The tree: ");
    tree_print(root);
    printf("\n");

    root = tree_remove(root, 45);

    printf("[3] The tree: ");
    tree_print(root);
    printf("\n");

    if (NULL != tree_search(root, 20))
    {
        printf("[4] The tree: ");
        tree_print(root);
        printf("\n");
    }

    root = tree_destroy(root);

    printf("[5] The tree: ");
    tree_print(root);
    printf("\n");

    return 0;
}