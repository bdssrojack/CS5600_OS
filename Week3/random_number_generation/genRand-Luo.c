/*
* genRand-Luo.c / Week3 random number generation
*
* Zhongshi Luo / CS5600 / Northeastern University
* Spring 2024 / Jan 29, 2024
*
*/
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <sys/time.h>

#define A 17U
#define C 21U
#define M (1<<8)

/**
 * return a random integer in the range from l to h inclusive
*/
int genRand(int l, int h) {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    unsigned int seed = (unsigned int)(tv.tv_usec);
    seed = (A * seed + C) % M;
    return seed % (h - l + 1) + l;
}

int lineCnt(char* fileName) {
    FILE* file = fopen(fileName, "r");
    if(file == NULL)
        return -1;

    int cnt = 0;
    int c;

    do{
        c = fgetc(file);
        if(c == '\n')
            cnt++;
    }while (c != EOF);

    fclose(file);
    return cnt;
}

int main(int argc, char* argv[]){
    if(argc <= 2 || argc > 4){
        printf("Usage: ./genRand [number of random integers to generate] [file to write generated integer into] [option to append or overwrite]\n");
        return 1;
    }

    int num = atoi(argv[1]);
    if(num <= 0){
        printf("A non-zero positive integer must be provided.\n");
        return 1;
    }

    char* fileName = argv[2];
    FILE *file;

    int before = 0;

    if(argc == 4 && strcmp("-a", argv[3]) == 0){
        printf("appending to file %s\n", fileName);
        before = lineCnt(fileName);
        file = fopen(fileName, "a");
    }else{
        file = fopen(fileName, "w");
    }

    if(file == NULL) {
        perror("Error:");
        return 1;
    }

    for (int i = 0; i < num; i++) {
        int r = genRand(1, 100);
        fprintf(file, "%d\n", r);
    }

    fclose(file);

    int after = lineCnt(fileName); // get the line number after write

    /*test*/
    if(num == after - before){
        printf("Successfully wrote %d lines to file %s.\n", num, fileName);
    }else{
        printf("Failed, before: %d, after: %d, line wrote: %d\n", before, after, after - before);
    }

    return 0;
}