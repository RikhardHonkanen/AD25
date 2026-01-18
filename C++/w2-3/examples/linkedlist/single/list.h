/**
 * @file list.h
 * @author Faroch Mehri (faroch.mehri@ya.se)
 * @brief A single instance linked list.
 * @version 0.1
 * @date 2021-05-23
 *
 * @copyright Copyright (c) 2021
 *
 */

#ifndef LIST_H
#define LIST_H

#include <cstddef>

namespace list
{
    /**
     * @brief This function is used to insert an element in the list.
     *
     * @param data The integer to insert.
     * @return bool - true if value is inserted successfully; otherwise false
     */
    bool insert(int data);

    /**
     * @brief This function returns number of the elements in the list.
     *
     * @return size_t - Number of the data elements stored in the list.
     */
    size_t available(void);

    /**
     * @brief This function is used to search for a data in the list.
     *
     * @param data The integer to search for.
     * @return bool - true if data is in the list; otherwise false.
     */
    bool search(int data);

    /**
     * @brief This function is used to delete an element in the list
     *
     * @param data The data value of a node
     * @return bool - false if data does not exist; otherwise true
     */
    bool remove(int data);

    /**
     * @brief This function is used to change data stored in the list.
     *
     * @param odata The old data value
     * @param ndata The new data value
     * @return bool - false if odata does not exist int the list; otherwise true
     */
    bool edit(int odata, int ndata);

    /**
     * @brief This function is used to free the allocated memory for all the nodes.
     *
     */
    void clear(void);
}

#endif /* LIST_H */
