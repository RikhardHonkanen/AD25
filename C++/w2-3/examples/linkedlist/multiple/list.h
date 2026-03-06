/**
 * @file list.h
 * @author Faroch Mehri (faroch.mehri@ya.se)
 * @brief A multiple instance linked list.
 * @version 0.1
 * @date 2021-05-23
 *
 * @copyright Copyright (c) 2021
 *
 */

#ifndef LIST_H
#define LIST_H

struct list_t; // The forward declared type

namespace list
{

    /**
     * @brief This function is used to create a linked list.
     *
     * @return list_t* nullptr if an error occurrs else a pointer to the list.
     */
    list_t *create(void);

    /**
     * @brief This function is used to insert a data element in a list.
     *
     * @param lst The list to insert data to.
     * @param data The data to insert.
     * @return true if data is inserted successfully else false.
     */
    bool insert(list_t *lst, int data);

    /**
     * @brief This function is used to get number of the elements in a list.
     *
     * @param lst The list to get number of the elements.
     * @return int -1 if there is an error; otherwise number of the elements.
     */
    int available(list_t *lst);

    /**
     * @brief This function is used to search for a data in the list.
     *
     * @param lst The list to search in.
     * @param data The data to search for.
     * @return int true if data is in the list else false.
     */
    bool search(list_t *lst, int data);

    /**
     * @brief This function is used to delete a data element from a list.
     *
     * @param lst The list to delete data from.
     * @param data The data to delete.
     * @return true if data is deleted successfully else false.
     */
    bool remove(list_t *lst, int data);

    /**
     * @brief This function is used to edit a data element stored in a list.
     *
     * @param lst The list to edit.
     * @param odata The old data
     * @param ndata The new data to overwrite the old data
     * @return true if the old data is edited successfully else false.
     */
    bool edit(list_t *lst, int odata, int ndata);

    /**
     * @brief This function is used to remove all the nodes of a list
     *
     * @param lst Address of the list to remove its nodes
     */
    void clear(list_t *lst);

    /**
     * @brief This function is used to destroy a list.
     *
     * @param lst The list to destroy.
     */
    void destroy(list_t *&lst);
}

#endif /* LIST_H */
