#include <stdio.h>
#include <stdlib.h>
#include "list.h"

/*
* List implementation
*/
/*
 * Returns a newly created, empty list.
 */
list_t *list_create(void)
{
    list_t *myList = malloc(sizeof(list_t));    
    myList->head = NULL;
    myList->numItems = 0;
    return myList;
}

/*
 * Frees the list; list and nodes, but not the items it holds.
 */
void list_destroy(list_t *list)
{
    listnode_t *currentNode = list->head;
    listnode_t *nextNode;   
    while(1) {
        nextNode = currentNode->next;
        if (nextNode == NULL) {
            free(currentNode);
            free(list);
            if (currentNode != NULL || list != NULL)
                printf("list destroy memory free failed\n"); 
            break;
        }
        else {
            free(currentNode);
            if (currentNode != NULL)
                printf("list destroy memory free failed\n");
            currentNode = nextNode;
        }
    }
}

/*
 * Adds an item first in the provided list.
 */
void list_addfirst(list_t *list, void *item)
{
    listnode_t *node = malloc(sizeof(listnode_t));
    node->item = item;
    listnode_t *oldHead = list->head;
    list->head = node;

    if (list->numItems == 0){
        node->next = NULL;      
    }
    else {
        node->next = oldHead;
    }
    
    list->numItems += 1;
}

/*
 * Adds an item last in the provided list.
 */
void list_addlast(list_t *list, void *item)
{
    listnode_t *currentNode = list->head;
    listnode_t *node = malloc(sizeof(listnode_t));
    node->next = NULL;
    node->item = item;
    while(1) {
        //No items in list
        if (list->numItems == 0){
            list->head = node;
            list->numItems ++;
            break;
        } else if (currentNode->next == NULL){
            currentNode->next = node;
            list->numItems ++;
            break;
        } else {
            currentNode = currentNode->next;
        }
    }
}
/*
 * Removes an item from the provided list, only freeing the node.
 */
void list_remove(list_t *list, void *item)
{   
    listnode_t *prevNode = NULL;
    listnode_t *currentNode = list->head;
    while(1) {
        if (currentNode->item == item) {
            if (currentNode == list->head) {
                list->head = currentNode->next;
                free(currentNode);
                list->numItems --;
                break;
            }
            else {
                prevNode->next = currentNode->next;
                free(currentNode);
                list->numItems --;
                break;
            }
        }
        else {
            prevNode = currentNode;
            currentNode = currentNode->next;
        } 
    }
}


/*
 * Return the number of items in the list.
 */
int list_size(list_t *list)
{
    return(list->numItems);
}

/*
 * Iterator implementation
 */

/*
 * Return a newly created list iterator for the given list.
 */
list_iterator_t *list_createiterator(list_t *list)
{
    list_iterator_t *iter = malloc(sizeof(list_iterator_t));
    iter->list = list;
    iter->next = list->head;
    
    return iter;
}


/*
 * Free the memory for the given list iterator.
 */
void list_destroyiterator(list_iterator_t *iter)
{
    free(iter);
}


/*
 * Move iterator to next item in list and return current.
 */ 
void *list_next(list_iterator_t *iter)
{
    void *cItem = iter->next->item;
    iter->next = iter->next->next;
    //(printf("%p\n", iter->next);

    //if list is at last position, reset
    if(iter->next == NULL) 
        list_resetiterator(iter);
    return cItem;
}



/*
 * Let iterator point to first item in list again.
 */
void list_resetiterator(list_iterator_t *iter)
{
    iter->next = iter->list->head;
}
