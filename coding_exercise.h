#ifndef CODING_EXERCISE_H_INCLUDED
#define CODING_EXERCISE_H_INCLUDED
/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

/*===========================================================================

            LOCAL DEFINITIONS AND DECLARATIONS FOR MODULE

  This section contains local definitions for constants, macros, types,
  variables and other items needed by this module.

===========================================================================*/
#define SIZE_ELEMENT     1
#define NUM_ELEMENT      1
#define MAX_QUEUE_SIZE   32
#define MAX_12BIT_LIMIT  4095

#define TRUE             1
#define FALSE            0

typedef unsigned short uint16_t;

/* A Singly linked list node to store a queue entry */
typedef struct QueueNode_s {
    uint16_t           key;
    struct QueueNode_s *next;
}QueueNode;

/* The queue stores the last 32 elements read from the file */
typedef struct QueueList_s {
    QueueNode *front;
    QueueNode *rear;
    char      count;
}QueueList;

/*===========================================================================

FUNCTION: sort

DESCRIPTION:
  This function sorts the array of 32 elements using the quicksort algorithm
  with last array element as the pivot.

DEPENDENCIES:
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void sort(uint16_t arr[], int l, int h);

/*===========================================================================

FUNCTION: partition

DESCRIPTION:
  This function is a utility function used by the sort to partition the array
  this function selects the last element as the pivot and splits the array
  in place.

DEPENDENCIES:
  None.

RETURN VALUE:
  int: Array partition index.

SIDE EFFECTS:
  None.
===========================================================================*/
int partition (uint16_t arr[], int l, int h);

/*===========================================================================

FUNCTION: swap

DESCRIPTION
  This function is a utility function used by the sort to swap integers in
  the list.

DEPENDENCIES:
  None.

RETURN VALUE:
  None.

SIDE EFFECTS:
  None.
===========================================================================*/
void swap(uint16_t* a, uint16_t* b);

/*===========================================================================

FUNCTION: merge

DESCRIPTION
  This function merges the currently read 32 integers to the maximum 32 value
  array, this merger is done for every 32 values read.

DEPENDENCIES:
  None.

RETURN VALUE:
  None.

SIDE EFFECTS:
  None.
===========================================================================*/
void merge(uint16_t a[], uint16_t b[], int count);

/*===========================================================================

FUNCTION: enQueue

DESCRIPTION
  This function adds 12-bit integers to the queue as they are read from the
  input *.bin file.

DEPENDENCIES:
  None.

RETURN VALUE:
  None.

SIDE EFFECTS:
  None.
===========================================================================*/
void enQueue(QueueList *q, uint16_t k);

/*===========================================================================

FUNCTION: deQueue

DESCRIPTION:
  This function removes the 12-bit integers added to the queue if the queue
  is reached the limit of 32 or when the queue elements are to be written
  to the file.

DEPENDENCIES:
  None.

RETURN VALUE:
  QueueNode: Rear element of the queue.

SIDE EFFECTS:
  None.
===========================================================================*/
QueueNode *deQueue(QueueList *q);

/*===========================================================================

FUNCTION: createQueue

DESCRIPTION:
  This function intializes the QueueList to set the values to default.

DEPENDENCIES:
  None.

RETURN VALUE:
  QueueList: return the pointer to the QueueList.

SIDE EFFECTS:
  None.
===========================================================================*/
QueueList *createQueue();

/*===========================================================================

FUNCTION: newNode

DESCRIPTION:
  This function create the newNode elements to be added to the QueueList.

DEPENDENCIES:
  None.

RETURN VALUE:
  QueueNode: pointer to the QueueNode element.

SIDE EFFECTS:
  None.
===========================================================================*/
QueueNode* newNode(uint16_t k);

#endif /* CODING_EXERCISE_H_INCLUDED */
