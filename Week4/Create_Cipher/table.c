#include "table.h"

table_t* newTable(int size) {
    table_t* table = (table_t*)malloc(sizeof(table_t));
    
    table->size = size;
    table->square = (char**)malloc(size * sizeof(char*));
    for(int i = 0; i < size; i++)
        table->square[i] = (char*)malloc(size * sizeof(char));

    return table;
}

void freeTable(table_t* table) {
    for(int i = 0; i < table->size; i++)
        free(table->square[i]);
    free(table->square);
    free(table);
}

int* searchTable(table_t* table, char c) {
    for(int i = 0; i < table->size; i++) {
        for(int j = 0; j < table->size; j++) {
            if(c == table->square[i][j]){
                int* res = (int*)malloc(2 * sizeof(int));
                res[0] = i;
                res[1] = j;
                return res;
            }
        }
    }
    return NULL;
}