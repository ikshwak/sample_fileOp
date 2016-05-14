/*===========================================================================
Problem Statement:
==================
Write an ANSI C program which takes as arguments the name of a binary input
file and a text output file. The binary input file will contain 12 bit
unsigned values.  The output file should contain the 32 largest values from
the input file. It should also contain the last 32 values read from the input
file.

Build Instructions:
===================
compile:
    gcc -Wall -g -ansi  -c coding_exercise.c -o coding_exercise.o
Link:
    gcc -o coding_exercise coding_exercise.o
execute:
    ./coding_exercise.exe test1.bin test1.out

Design Idea:
============
Introduction:

The scope of this design document is to explain the working and specifications of
the code delivered as a part of the coding exercise. The problem states to write
an ANSI C program which takes as arguments the name of a binary input file and a
text output file.  The binary input file will contain 12 bit unsigned values.  The
output file should contain the 32 largest values from the input file.  It should
also contain the last 32 values read from the input file.
The output file format should be as follows
•	Start with “--Sorted Max 32 Values--"
•	The 32 largest values in the file, one value per line.  Duplicates are allowed
    if a large value appears multiple times.
•	The list should be sorted smallest to largest.
•	Output “--Last 32 Values--" on its own line
•	The last 32 values read, one per line.
•	They should appear in the order they were read.
•	The last value read from the file will appear last.

Design Specifications:

The output specifies to read the 12 bit integers from the binary file and store the
maximum 32 integers and the last 32 integers to the output file.

1.	Maximum 32 integers: The binary file can have 12 bit integers of any number
    (as duplicates are allowed). Hence reading all the integer values from the
    file and loading those on memory (either stack or heap) will not be feasible
    for a low memory device. As we only need the maximum 32 values the following
    approach consumes the least memory:

    •	Read 3 bytes of binary data from the file; in case of odd number of 12-bit
        integers the last nibble will be ignored.
    •	Convert the first 3 nibbles to an unsigned short integer and convert the
        second 3 nibbles to and unsigned short integer.
    •	Globally define 2 unsigned short integer array of length 32.
    •	Curr32[] – stores the recently read 32 12-bit integers.
    •	Max32[] – stores the maximum 32 12-bit integers.
    •	Store these values to a predefined unsigned short integer array of length
        32 – curr32[]
    •	Check if the curr32[] is full; if full Sort the values in curr32[] array
        and merge them with max32[] in increasing order to update the max32[] with
        the latest maximum values; Reset the values in curr32[] to read the next set
        of 32 values.
    •	Repeat these steps until all the bytes are read from the input bin file.
    •	Finally sort and merge the left over values from the curr32[] array to max32[].

2.	Last 32 integers: The idea is save only the last 32 integers so there is no need to
    store the data from the entire file to memory. The following approach uses the dynamic
    memory to save the last 32 values:

    •	Read 3 bytes of binary data from the file; in case of odd number of 12-bit integers
        the last nibble will be ignored.
    •	Convert the first 3 nibbles to an unsigned short integer and convert the second 3
        nibbles to and unsigned short integer.
    •	These integers are added to dynamically allocated Queue data structure with a maximum
        size of 32.
    •	As the queue size exceeds the earliest read element is de-queued and the latest read
        element is en-queued.
    •	At end of the read, the Queue holds 32 last read values or as many integers if they
        are less than 32 integers in the input bin file.

Memory consumption:

Stack memory: The maximum 32 integers algorithm uses stack.

1.	Globally allocated 2 unsigned short integer array of length 32 –
    32*2*sizeof(unsigned short) = 128 bytes of constant memory.
2.	Temporarily allocated 1 unsigned short integer array of length 64 –
    64* sizeof(unsigned short) = 128 bytes of constant memory.

Heap memory: The last 32 integers algorithm uses heap.
1.	QueueList: 12 bytes of constant memory.
2.	QueueNode: 32*8 = 256 bytes of maximum memory.

Performance:
1.	Maximum 32 integers:
a.	Sorting of 32 integers is O(32log32) – constant time.
b.	Merging of 32 + 32 integers O(32+32) – constant time.
c.	Data read from file is O(n) – depends on the file size.
2.	Last 32 integers:
a.	The Queue en-queue and de-queue operation are O(1) – constant time.

Tradeoffs:
With low memory constraints storing the maximum 32 values takes more time
to read every 32 values from the file, sort then and merge them to the max
values array.
With higher flexibility in memory, we can use a counting sort technique to
store the values read from the files and save max 32 values to the output file.
The time complexity would be O(1) overall but the memory consumption would be
array of integers of size 4096 – 4096*4 = 16KB of constant memory on stack.

===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "coding_exercise.h"

/*-------------------------------------------------------------------------
            Local Variables
-------------------------------------------------------------------------*/
uint16_t max32[32]   = {0};
uint16_t curr32[32]  = {0};
int      curr32Count = 0;
int      max32Count  = 0;


/*===========================================================================

FUNCTION: Main Routine

DESCRIPTION
  This function takes as arguments the name of a binary input file and a text
  output file. The binary input file will contain 12 bit unsigned values.
  The output file should contain the 32 largest values from the input file.
  It should also contain the last 32 values read from the input file.

DEPENDENCIES:
  None.

RETURN VALUE:
  SUCCESS: 0
  FAILURE: -1

SIDE EFFECTS:
  None.
===========================================================================*/
int main(int argc, char **argv)
{
    FILE           *fd      = 0;
    unsigned char  buff[3]  = {0};
    uint16_t       first;
    uint16_t       second;
    QueueNode      *node;
    QueueList      *last32q;
    int            result   = 0;

    /* argv[1] - name of the input file, argv[2] - name of the output file*/
    if(argc != 3)
    {
        printf("insufficient number of arguments\n");
        return -1;
    }
    /* open the input file to read the binary data*/
    fd = fopen(argv[1],"r");
    if(0 == fd)
    {
        printf("\n fopen() Error!!!\n");
        return -1;
    }

    /* Create the queue to store the last 32 values read from the input file*/
    last32q = createQueue();
    if(0 == last32q)
    {
        printf("\n createQueue() Error!!!\n");
        fclose(fd);
        return -1;
    }

    do
    {
        int  i = 0;
        unsigned char c = 0;
        memset(buff, 0, 3*sizeof(unsigned char));
        for(i = 0; i < 3; i++)
        {
            /* Read 3 bytes at a time to convert them into 2 12-bit unsigned integers*/
            if(SIZE_ELEMENT*NUM_ELEMENT == fread(&c,SIZE_ELEMENT,NUM_ELEMENT,fd))
            {
                buff[i] = c;
            }
            else
            {
                /*If the fread reaches feof*/
                if(ferror(fd))
                {
                    printf("file read error\n");
                    result = -1;
                    goto ExitCondition;
                }
                break;
            }
        }
        /*                  byte format read from the file
                                     xx xx xx
           first 12-bit integer<---[xx x][x xx]----> second 12-bit integer*/
        if((i == 0) && feof(fd))
        {
            /*fread() reached feof*/
            continue;
        }
        else
        {
            /*If there are an odd number of 12bit readings in the file,
                  then the last nibble in the file will be zero and can be ignored.*/
            first = (buff[ 0 ] << 4)| ((buff[ 1 ] & 0xF0) >> 4);
            if(first > MAX_12BIT_LIMIT)
            {
                printf("invalid integer range\n");
                result = -1;
                goto ExitCondition;
            }
            /*Add the unsigned integers to the queue in the order they are read*/
            enQueue(last32q, first);
            /*Add the unsigned integers to the array to sort elements*/
            curr32[curr32Count++] = first;
            if(i == 3)
            {
                /*there is a valid second 12-bit integer*/
                second = ((buff[ 1 ] & 0x0F) << 8) | buff[ 2 ];
                if(second > MAX_12BIT_LIMIT)
                {
                    printf("invalid integer range\n");
                    result = -1;
                    goto ExitCondition;
                }
                /*Add the unsigned integers to the queue in the order they are read*/
                enQueue(last32q, second);
                /*Add the unsigned integers to the array to sort elements*/
                curr32[curr32Count++] = second;
            }
        }
        if(curr32Count == MAX_QUEUE_SIZE)
        {
            /*sort 32 elements read from the file and merge with the max32 list*/
            sort(curr32, 0, MAX_QUEUE_SIZE - 1);
            merge(curr32, max32, MAX_QUEUE_SIZE);
            memset(curr32, 0, MAX_QUEUE_SIZE*sizeof(uint16_t));
            curr32Count = 0;
            max32Count = MAX_QUEUE_SIZE;
        }
    }while(!feof(fd));

    /*sort the remaining elements read from the file and merge with the max32 list*/
    sort(curr32, 0, curr32Count - 1);
    merge(curr32, max32, curr32Count);
    memset(curr32, 0, MAX_QUEUE_SIZE*sizeof(uint16_t));
    curr32Count = 0;

    /*Close the input file after reading and processing the data*/
    if(fd != 0)
    {
        fclose(fd);
        fd = 0;
    }

    /*Open the output file -- argv[2] to write last 32 elements
      and highest 32 elements in the specified format*/
    fd = fopen(argv[2], "w+");
    if(0 == fd)
    {
        printf("\n fopen() Error!!!\n");
        result = -1;
        goto ExitCondition;
    }

    /*Add the "--Sorted Max 32 Values--" header string to the output file*/
    if(fprintf(fd, "--Sorted Max 32 Values--\n") < 0)
    {
        printf("\n fprintf() Error!!!\n");
        result = -1;
        goto ExitCondition;
    }

    {
        int i = 0;
        for(i = 0; i < max32Count; i++)
        {
            if(fprintf(fd, "%d\n", max32[i]) < 0)
            {
                printf("\n fprintf() Error!!!\n");
                result = -1;
                goto ExitCondition;
            }
        }
    }
    /*Add the "--Last 32 Values--" header string to the output file*/
    if(fprintf(fd, "--Last 32 Values--\n") < 0)
    {
        printf("\n fprintf() Error!!!\n");
        result = -1;
        goto ExitCondition;
    }

    /*deQueue the elements from the last32 queue and write to the file in the order they are read*/
    while(0 != (node = deQueue(last32q)))
    {
        if(fprintf(fd, "%d\n", node->key) < 0)
        {
            printf("\n fprintf() Error!!!\n");
            result = -1;
            goto ExitCondition;
        }
        free(node);
        node = 0;
    }

ExitCondition:
    /*Close the output file after the write is completed*/
    if(0 != fd)
    {
        fclose(fd);
        fd = 0;
    }

    /*Release the allocated last32 queue memory in heap*/
    if(0 != last32q)
    {
        /*Release the allocated queue nodes memory in heap*/
        while(0 != (node = deQueue(last32q)))
        {
            free(node);
            node = 0;
        }
        free(last32q);
        last32q = 0;
    }
    max32Count  = 0;
    curr32Count = 0;

    return result;
}

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
void sort(uint16_t arr[], int l, int h)
{
    if (l < h)
    {
        int p = partition(arr, l, h); /* Partitioning index */
        sort(arr, l, p - 1);
        sort(arr, p + 1, h);
    }
}

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
int partition (uint16_t arr[], int l, int h)
{
    int x = arr[h];
    int i = (l - 1);
    int j = 0;

    for (j = l; j <= h- 1; j++)
    {
        if (arr[j] <= x)
        {
            i++;
            swap(&arr[i], &arr[j]);
        }
    }
    swap(&arr[i + 1], &arr[h]);
    return (i + 1);
}

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
void swap(uint16_t* a, uint16_t* b)
{
    int t = *a;
    *a = *b;
    *b = t;
}

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
void merge(uint16_t a[], uint16_t b[], int count)
{
  uint16_t temp[64] = {0};
  int i, j, k;

  j = k = 0;

  /*If there are fewer than 32 values in the file then whatever is read should be output*/
  if(max32Count == 0)
  {
      for(i = 0; i < count; i++)
      {
          b[i] = a[i];
      }
      max32Count = count;
      return;
  }
  /*else if((max32Count == MAX_QUEUE_SIZE) && (count < MAX_QUEUE_SIZE))
  {

  }*/
  else
  {
      /*Merge the curr32 and max32 to a temp array to get a list of max 64 elements
      for every 32 12-bit integers read from the binary file*/
      for (i = 0; i < (MAX_QUEUE_SIZE*2);) {
        if (j < curr32Count && k < MAX_QUEUE_SIZE) {
          if (a[j] < b[k]) {
            temp[i] = a[j];
            j++;
          }
          else {
            temp[i] = b[k];
            k++;
          }
          i++;
        }
        else if (j < curr32Count) {
          for (; i < (MAX_QUEUE_SIZE*2);) {
            temp[i] = a[j];
            j++;
            i++;
          }
        }
        else if (k < MAX_QUEUE_SIZE) {
          for (; i < (MAX_QUEUE_SIZE*2);) {
            temp[i] = b[k];
            k++;
            i++;
          }
        }
      }
      /*Copy the maximum 32 values from the merged temp array to
      the global max32 array, update this array for every 32 integers read*/
      j = curr32Count;
      for(i = 0; i < MAX_QUEUE_SIZE; i++, j++)
      {
          b[i] = temp[j];
      }
  }
}

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
void enQueue(QueueList *q, uint16_t k)
{
    /* Create a new LL node*/
    QueueNode *temp = 0;
    /* If the queue is full remove the rear element to add new element */
    if(q->count == MAX_QUEUE_SIZE)
    {
        temp = deQueue(q);
        if(temp)
        {
            free(temp);
        }
        temp = 0;
    }
    temp = newNode(k);
    if(temp == 0)
    {
        printf("Failed to create NewNode()\n");
        return;
    }

    /* Increment the queue counter value to maintain the number of elements*/
    q->count++;
    /* If queue is empty, then new node is front and rear both*/
    if (q->rear == 0)
    {
       q->front = q->rear = temp;
       return;
    }
    /* Add the new node at the end of queue and change rear*/
    q->rear->next = temp;
    q->rear = temp;
}

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
QueueNode *deQueue(QueueList *q)
{
    QueueNode *temp = 0;
    /* If queue is empty, return 0.*/
    if ((q->front == 0) || (q == 0))
    {
       return 0;
    }

    q->count--;
    /* Store previous front and move front one node ahead*/
    temp = q->front;
    q->front = q->front->next;

    /* If front becomes 0, then change rear also as 0*/
    if (q->front == 0)
    {
        q->rear = 0;
    }
    return temp;
}

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
QueueList *createQueue()
{
    QueueList *q = (QueueList*)malloc(sizeof(QueueList));
    if(q == 0)
    {
        printf("failed to createQueue()\n");
        return 0;
    }
    q->front = q->rear = 0;
    q->count = 0;
    return q;
}


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
QueueNode* newNode(uint16_t k)
{
    QueueNode *temp = (QueueNode*)malloc(sizeof(QueueNode));
    if(temp == 0)
    {
        printf("failed to newNode()\n");
        return 0;
    }
    temp->key = k;
    temp->next = 0;
    return temp;
}
