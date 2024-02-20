#include <stdlib.h>
#include <string.h>
#include <stdio.h>

int main() {
    char* block = calloc(2048, sizeof(char));
    if(block)
        memset(block, 0xFF, 2048);
    printf("%s\n", block);
    free(block);

    return 0;
}