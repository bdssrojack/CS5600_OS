/*
* polybius.c / Week4 Create Cipher
*
* Zhongshi Luo / CS5600 / Northeastern University
* Spring 2024 / Feb 4, 2024
*
*/

#include "polybius.h"

static bool isLetter(char c){
    if((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))
        return true;
    return false;
}

static char capitalize(char c) {
    if(isLetter(c) && c <= 'z' && c >= 'a'){
        return 'A' + (c - 'a');
    }
    return c;
}

char* pbEncode(const char *plaintext, table_t* table) {
    // process the char in plain text string one by another
    int len = strlen(plaintext);
    char* strBuffer = (char*)malloc(2 * len * sizeof(char) + 1);
    strBuffer[0] = '\0'; // initialize buffer to an empty string

    for(int i = 0; i < len; i++) {
        char c = plaintext[i];
        c = capitalize(c);
        int* coord = searchTable(table, c);
        if(coord){
            // append the number to buffer
            char numStr[2];
            numStr[0] = coord[0]+'0';
            numStr[1] = coord[1]+'0';
            strcat(strBuffer, numStr);
            free(coord);
        }
    }

    return strBuffer;
}

char* pbDecode(const char *ciphertext, table_t* table) {
    int len = strlen(ciphertext);
    if(len % 2 != 0) {
        printf("Invalid cipher text.\n");
        return NULL;
    }

    char* strBuffer = (char*)malloc((len / 2) * sizeof(char) + 1);

    int cnt = 0;
    for(int i = 0; i < len;) {
        int row = ciphertext[i++]-'0';
        int col = ciphertext[i++]-'0';

        if(row >= table->size || row < 0 || col >= table->size || col < 0){
            printf("Invalid input, index exceed table boundary.\n");
            return NULL;
        }

        char c = table->square[row][col];
        strBuffer[cnt++] = c;
    }
    strBuffer[cnt] = '\0';

    return strBuffer;
}