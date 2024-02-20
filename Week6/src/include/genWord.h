#ifndef GENWORD_H
#define GENWORD_H

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>

#define A 17U
#define C 21U
#define M (1<<8)
#define MAXLEN 10U
#define MINLEN 3U

/**
 * generate a random seed
*/
unsigned int genSeed();

/**
* generate a random integer between h and l
*/
int genRand(int l, int h);

/**
 * return a random word
*/
char* genWord();

/**
 * @brief Get the number of lines in a file
 * 
 * @param fileName 
 * @return int number of lines in the file
 */
int lineCnt(char* fileName);

#endif