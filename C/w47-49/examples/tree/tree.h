/**
 * @file tree.h
 * @author Faroch Mehri (faroch.mehri@ya.se)
 * @brief An example of a binary search tree(BST).
 * @version 0.1
 * @date 2022-03-15
 *
 * @copyright Copyright (c) 2022
 *
 */
#ifndef TREE_H
#define TREE_H

typedef struct node node_t; // The forward declared ADT

/**
 * @brief This function is used to insert a node in a tree
 *
 * @param root A pointer to the root of the tree
 * @param key The data to store
 * @return node_t* A pointer to the root of the tree
 */
node_t *tree_insert(node_t *root, int key);

/**
 * @brief This function is used to search for a specific data element in a tree
 *
 * @param root A pointer to the root of the tree
 * @param key The data to search for
 * @return node_t* NULL if key does not exist else a pointer to the found node
 */
node_t *tree_search(const node_t *root, int key);

/**
 * @brief This function is used to delete a specific data element in a tree
 *
 * @param root A pointer to the root of the tree
 * @param key The data to delete
 * @return node_t* A pointer to the root of the tree
 */
node_t *tree_remove(node_t *root, int key);

/**
 * @brief This function is used to print a tree
 *
 * @param root A pointer to the root of the tree
 */
void tree_print(const node_t *root);

/**
 * @brief This function is used to destroy a tree
 *
 * @param root A pointer to the root of the tree
 * @return node_t* A NULL pointer
 */
node_t *tree_destroy(node_t *root);

#endif