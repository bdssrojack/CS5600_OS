#include "queue.h"

/**
 * @brief Create a Node object
 * 
 * @param data 
 * @return node_t* 
 */
static node_t* createNode(void* data, data_type type){
    node_t* node = (node_t*)malloc(sizeof(node_t));

    node->type = type;
    node->data = data;
    node->next = NULL;
    node->prev = NULL;

    return node;
}

/**
 * @brief Delete a Node object
 * 
 * @param node 
 */
static void deleteNode(node_t* node){
    if(node->type == DATA_TYPE_PROCESS){
        process_t* p = (process_t*)node->data;
        free(p->name);
    }
    free(node->data);
    free(node);
}

process_t* createProcess(int id, char* name, long rt, int pr){
    printf("Creating a process\n");
    process_t* p = (process_t*)malloc(sizeof(process_t));

    p->id = id;
    p->name = name;
    p->rt = rt;
    p->pr = pr;

    return p;
}

queue_t* createQ(){
    printf("Creating a queue\n");
    queue_t* queue = (queue_t*)malloc(sizeof(queue_t));

    queue->head = (node_t*)malloc(sizeof(node_t));
    queue->tail = (node_t*)malloc(sizeof(node_t));

    queue->head->next = queue->tail;
    queue->head->prev = NULL;
    queue->tail->prev = queue->head;
    queue->tail->next = NULL;

    queue->size = 0;

    return queue;
}

void deleteQ(queue_t* queue){
    printf("Deleting a queue\n");
    node_t* current = queue->head->next;

    while(current != queue->tail){
        node_t* temp = current;
        current = current->next;
        deleteNode(temp);
    }

    free(queue->head);
    free(queue->tail);

    free(queue);
}

void printQ(queue_t* queue){
    if(queue->size == 0){
        printf("Queue is empty\n");
        return;
    }

    node_t* current = queue->head->next;
    int cnt = 0;

    printf("---------- Queue Status ----------\n");
    printf("Queue size: %d\n", queue->size);

    while(current != queue->tail){
        process_t* p = (process_t*)current->data;
        printf("%d -- id: %d, name: %s, rt: %ld, pr: %d\n",cnt++, p->id, p->name, p->rt, p->pr);
        current = current->next;
    }

    printf("---------------------------------\n");
}

void add2q(queue_t* queue, void* element){
    printf("Adding to queue\n");
    // TODO: specify the data type by parameter
    node_t* node = createNode(element, DATA_TYPE_PROCESS);

    node->next = queue->tail;
    node->prev = queue->tail->prev;
    node->prev->next = node;
    queue->tail->prev = node;

    queue->size++;
}

void* popQ(queue_t* queue){
    printf("Popping from queue\n");
    if(queue->size == 0){
        return NULL;
    }

    node_t* node = queue->head->next;
    void* data = node->data;
    queue->head->next = node->next;
    node->next->prev = queue->head;
    deleteNode(node);

    queue->size--;

    return data;
}

process_t* rmProcess(queue_t* queue){
    printf("Removing the process with the highest priority\n");
    if(queue->size == 0){
        return NULL;
    }

    node_t* current = queue->head->next;
    node_t* max = current;

    while(current != queue->tail){
        process_t* p = (process_t*)current->data;
        process_t* m = (process_t*)max->data;
        if(p->pr > m->pr){
            max = current;
        }
        current = current->next;
    }

    process_t* data = (process_t*)max->data;
    max->prev->next = max->next;
    max->next->prev = max->prev;
    deleteNode(max);

    queue->size--;

    return data;
}

int qsize(queue_t* queue){
    return queue->size;
}

