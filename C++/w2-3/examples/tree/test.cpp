#include "tree.h"
#include <cstdio>

int main(void)
{
    node_t *root = nullptr;

    root = tree::insert(root, 50);
    root = tree::insert(root, 30);
    root = tree::insert(root, 20);
    root = tree::insert(root, 40);
    root = tree::insert(root, 70);
    root = tree::insert(root, 60);
    root = tree::insert(root, 80);

    (void)printf("[1] The tree: ");
    tree::print(root);
    (void)printf("\n");

    root = tree::insert(root, 45);

    (void)printf("[2] The tree: ");
    tree::print(root);
    (void)printf("\n");

    root = tree::remove(root, 45);

    (void)printf("[3] The tree: ");
    tree::print(root);
    (void)printf("\n");

    if (nullptr != tree::search(root, 20))
    {
        (void)printf("[4] The tree: ");
        tree::print(root);
        (void)printf("\n");
    }

    root = tree::destroy(root);

    (void)printf("[5] The tree: ");
    tree::print(root);
    (void)printf("\n");

    return 0;
}