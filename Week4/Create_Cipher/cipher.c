/*
* cipher.c / Week4 Create Cipher
* 
* Takes input from command line, outputs the encrypted or decrypted text
*
* Zhongshi Luo / CS5600 / Northeastern University
* Spring 2024 / Feb 4, 2024
*
*/
#include "polybius.h"

int main(int argc, char* argv[]) {
    if(argc < 3){
        printf("Usage: ./cipher -d/e <your_input>");
        return 1;
    }

    // initialize a 6*6 square table
    int size = 6;
    table_t* table = newTable(size);
    char c = 'A';
    for(int i = 0; i < table->size; i++) {
        for(int j = 0; j < table->size; j++) {
            table->square[i][j] = c++;
        }
    }

    if(strcmp(argv[1], "-d") == 0) {
        printf("Decoded text: ");
        for(int i = 2; i < argc; i++) {
            char* decoded = pbDecode(argv[i], table);
            printf("%s ", decoded);
            free(decoded);
        }
        printf("\n");
    }

    if(strcmp(argv[1], "-e") == 0) {
        printf("Encoded text: ");
        for(int i = 2; i < argc; i++) {
            char* encoded = pbEncode(argv[i], table);
            printf("%s ", encoded);
            free(encoded);
        }
        printf("\n");
    }

    freeTable(table);
    return 0;
}