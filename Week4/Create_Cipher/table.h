/**
 * The polybius cipher table struct, a square 2-d char array
*/
#ifndef TABLE_T
#define TABLE_T

#include <stdlib.h>

/**
 * table structure
 * char** square: 2d array of char
 * int size: row and column number of square
*/
typedef struct table
{
    char** square;
    int size;
}table_t;

/**
 * table constructor
*/
table_t* newTable(int size);

/**
 * table destructor
*/
void freeTable(table_t* table);

/**
 * search the given table for character
 * @return int array [row, col] or NULL if not found
*/
int* searchTable(table_t* table, char c);

#endif