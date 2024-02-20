#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define MAX_LINE 10 // maximum number of line in data file
#define MAX_NUM 100 // maximum number of integer per line
#define BUFFER 1024 // line read buffer size

typedef struct dynBlock {
    /*dynamically allocated integer array*/
    int* arr;
    int size;
}dynBlock;

/**
 * Creat a dynBlock object with an int array pointer of given size
*/
dynBlock* allocDynBlock(int size) {
    dynBlock* blockPtr = (dynBlock*)malloc(sizeof(dynBlock));
    if(blockPtr == NULL)
        return NULL;

    blockPtr->arr = (int*)malloc(sizeof(int) * size);
    if(blockPtr->arr == NULL){
        free(blockPtr);
        return NULL;
    }
    blockPtr->size = size;

    return blockPtr;
}

/**
 * Store integers in array into a dynBlock
*/
dynBlock* storeMem2Blk(int arr[], int size) {
    dynBlock* blk = allocDynBlock(size);
    if(blk == NULL)
        return NULL;

    memcpy(blk->arr, arr, size * sizeof(int));

    return blk;
}

void clean(dynBlock* blockArr[], int size) {
    for(int i = 0; i < size; i++) {
        free(blockArr[i]->arr);
    }
}

int main() {
    dynBlock* blockArr[MAX_LINE]; //store the dynBlock pointers

    FILE* data = fopen("blocks.data", "r");
    if(data == NULL) {
        perror("Error open file ");
        return 1;
    }

    int numBuffer[MAX_NUM];
    int numCnt = 0;
    char lineBuffer[BUFFER];
    int lineCnt = 0;

    /* Read data file line by line
    */
    while(fgets(lineBuffer, sizeof(lineBuffer), data)) {
        printf("Line %d: %s", lineCnt, lineBuffer);
        char* lineStr = lineBuffer;
        numCnt = 0;
        
        /*Split line by space*/
        char *t = strtok(lineBuffer, " ");

        while(t != NULL && numCnt < MAX_NUM) {
            numBuffer[numCnt++] = atoi(t);
            t = strtok(NULL, " ");
        }
        
        /*determine the line is read correctly by comparing the original line string with string of number read*/
        if(lineCnt < MAX_LINE) {
            blockArr[lineCnt] = storeMem2Blk(numBuffer, numCnt);
            char readStr[BUFFER];
            readStr[0] = '\0';
            
            for(int i = 0; i < blockArr[lineCnt]->size; i++) {
                int n = blockArr[lineCnt]->arr[i];
                char numStr[3];
                sprintf(numStr, "%d", n); //write integer into string
                strcat(readStr, numStr);
                if(i != blockArr[lineCnt]->size-1)
                    strcat(readStr, " ");  
            }
            strcat(readStr, "\n");
            printf("Read %d: %s", lineCnt, readStr);
            // if(strcmp(readStr, lineStr) == 0) {
            //     printf("Read Succeed\n");
            // }else{
            //     printf("Read failed\n");
            // }
            printf("--------------------------------\n");
        }

        lineCnt++;
    }

    clean(blockArr, lineCnt);
    return 0;
}