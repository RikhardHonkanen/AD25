#include <new>
#include "tree.h"
#include <iostream>

struct node_t
{
    struct node_t *right;
    struct node_t *left;
    int key;
};

node_t *tree::insert(node_t *root, int key)
{
    if (root == nullptr) // The tree is empty
    {
        root = new (std::nothrow) node_t{nullptr, nullptr, key};
    }
    else if (key < root->key) // Add the node to the left subtree
    {
        root->left = tree::insert(root->left, key);
    }
    else // Add the node to the right subtree
    {
        root->right = tree::insert(root->right, key);
    }

    return root;
}

node_t *tree::remove(node_t *root, int key)
{
    if (root != nullptr)
    {
        if (key > root->key)
        {
            root->right = tree::remove(root->right, key);
        }
        else if (key < root->key)
        {
            root->left = tree::remove(root->left, key);
        }
        else
        {
            if ((root->left == nullptr) && (root->right == nullptr)) // The node has no child
            {
                delete root;
                root = nullptr;
            }
            else if ((root->left == nullptr) && (root->right != nullptr)) // The node has only a right child
            {
                node_t *right = root->right;
                delete root;
                root = right;
            }
            else if ((root->left != nullptr) && (root->right == nullptr)) // The node has only a left child
            {
                node_t *left = root->left;
                delete root;
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

                while ((current != nullptr) && (current->left != nullptr))
                {
                    current = current->left;
                }

                root->key = current->key;

                root->right = tree::remove(root->right, root->key);
            }
        }
    }

    return root;
}

node_t *tree::search(const node_t *root, int key)
{
    node_t *current = nullptr;

    if (root != nullptr)
    {
        if (key == root->key)
        {
            current = const_cast<node_t *>(root);
        }
        else if (key > root->key)
        {
            current = tree::search(root->right, key);
        }
        else
        {
            current = tree::search(root->left, key);
        }
    }

    return current;
}

void tree::print(const node_t *root)
{
    if (root != nullptr)
    {
        tree::print(root->left);
        std::cout << root->key << "\t";
        tree::print(root->right);
    }
}

node_t *tree::destroy(node_t *root)
{
    if (root != nullptr)
    {
        tree::destroy(root->right);
        tree::destroy(root->left);
        delete root;
        root = nullptr;
    }

    return root;
}