#ifndef QUEUE
#define QUEUE

#include <stdlib.h>
#include <stdio.h>

/**
 * @brief a structure that contains the process id, name, runtime and priority.
 * 
 */
typedef struct process{
    int id;
    char* name;
    long rt;
    int pr;
}process_t;

/**
 * @brief element of the queue that holds any type of data.
 * 
 */
typedef struct node{
    void* data;
    struct node* next;
    struct node* prev;
}node_t;

/**
 * @brief a doubly-linked list contains any data.
 * 
 * Two sentinel nodes to represent the head and tail of the queue.
 */
typedef struct queue{
    node_t* head;
    node_t* tail;
    int size;
}queue_t;

/**
 * @brief adds one instance of an element to the end of the queue
 * 
 * @param queue 
 * @param element 
 */
void add2q(queue_t* queue, void* element);

/**
 * @brief removes and returns the first element of the queue
 * 
 * @param queue 
 * @return void* 
 */
void* popQ(queue_t* queue);

/**
 * @brief removes and returns the process with the highest priority in the queue
 * 
 * @param queue 
 * @return process_t* 
 */
process_t* rmProcess(queue_t* queue);

/**
 * @brief returns the number of elements in the queue
 * 
 * @param queue 
 * @return the number of elements in the queue; 0 if the queue is empty.
 */
int qsize(queue_t* queue);

/**
 * @brief Create a Queue
 * 
 * @return queue_t* 
 */
queue_t* createQ();

/**
 * @brief Delete a Queue
 * 
 * @param queue 
 */
void deleteQ(queue_t* queue);

/**
 * @brief print the status and contents of the queue
 * 
 * @param queue 
 */
void printQ(queue_t* queue);

/**
 * @brief Create a Process object
 * 
 * @param id 
 * @param name 
 * @param rt 
 * @param pr 
 * @return process_t* 
 */
process_t* createProcess(int id, char* name, long rt, int pr);

#endif