#include "tree.h"
#include <stdio.h>
#include <stdlib.h>

struct node
{
    struct node *right;
    struct node *left;
    int key;
};

node_t *tree_insert(node_t *root, int key)
{
    if (root == NULL) // The tree is empty
    {
        root = (node_t *)malloc(sizeof(node_t));

        if (root != NULL)
        {
            root->left = NULL;
            root->right = NULL;
            root->key = key;
        }
    }
    else if (key < root->key) // Add the node to the left subtree
    {
        root->left = tree_insert(root->left, key);
    }
    else // Add the node to the right subtree
    {
        root->right = tree_insert(root->right, key);
    }

    return root;
}

node_t *tree_remove(node_t *root, int key)
{
    if (root != NULL)
    {
        if (key > root->key)
        {
            root->right = tree_remove(root->right, key);
        }
        else if (key < root->key)
        {
            root->left = tree_remove(root->left, key);
        }
        else
        {
            if ((root->left == NULL) && (root->right == NULL)) // The node has no child
            {
                free(root);
                root = NULL;
            }
            else if ((root->left == NULL) && (root->right != NULL)) // The node has only a right child
            {
                node_t *right = root->right;
                free(root);
                root = right;
            }
            else if ((root->left != NULL) && (root->right == NULL)) // The node has only a left child
            {
                node_t *left = root->left;
                free(root);
                root = left;
            }
            else // The node has 2 children
            {
                /**
                 * @brief We need to find the minimum value in the right subtree of the node and assign it
                 *        to the value of the node we want to delete and then delete the node with the minimum
                 *        value in the right subtree of the node.                 *
                 */

                node_t *current = root->right;

                while ((current != NULL) && (current->left != NULL))
                {
                    current = current->left;
                }

                root->key = current->key;

                root->right = tree_remove(root->right, root->key);
            }
        }
    }

    return root;
}

node_t *tree_search(const node_t *root, int key)
{
    node_t *current = NULL;

    if (root != NULL)
    {
        if (key == root->key)
        {
            current = (node_t *)root;
        }
        else if (key > root->key)
        {
            current = tree_search(root->right, key);
        }
        else
        {
            current = tree_search(root->left, key);
        }
    }

    return current;
}

void tree_print(const node_t *root)
{
    if (root != NULL)
    {
        tree_print(root->left);
        printf("%d\t", root->key);
        tree_print(root->right);
    }
}

node_t *tree_destroy(node_t *root)
{
    if (root != NULL)
    {
        tree_destroy(root->right);
        tree_destroy(root->left);
        free(root);
        root = NULL;
    }

    return root;
}