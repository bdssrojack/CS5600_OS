/*
* genRand-Luo.c / Week6 random word generation
*
* Zhongshi Luo / CS5600 / Northeastern University
* Spring 2024 / Feb 18, 2024
*/
#include "genWord.h"

unsigned int genSeed(){
    struct timeval tv;
    gettimeofday(&tv, NULL);
    usleep(1); // sleep for 1us to avoid getting the same seed
    return (unsigned int)(tv.tv_usec);
}

int genRand(int l, int h){
    unsigned int seed = genSeed();
    seed = (A * seed + C) % M;
    return seed % (h - l + 1) + l;
}

char* genWord(){
    int len = genRand(MINLEN, MAXLEN);
    char* word = (char*)malloc(len + 1);
    for (int i = 0; i < len; i++) {
        word[i] = (char)genRand((int)'a', (int)'z');
    }
    word[len] = '\0';
    return word;
}

int lineCnt(char* fileName){
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