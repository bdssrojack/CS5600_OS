#ifndef POLYBIUS
#define POLYBIUS

#include <string.h>
#include <stdio.h>
#include <stdbool.h>

#include "table.h"

/**
 * Encodes string plaintext using the given cipher table.
 * English letters only, special characters are ignored.
 * 
 * @param plaintext plain text string to be encoded
 * @param table cipher table to be used for encoding
 * @return string of the encoded text
*/
char* pbEncode(const char *plaintext, table_t* table);

/**
 * Decodes ciphertext using the given polybius square table
 * 
 * @param ciphertext cipher text string to be decoded
 * @param table cipher table to be used for decoding
 * @return string of the decoded text in all upper case
*/
char* pbDecode(const char *ciphertext, table_t* table);

/**
 * Encodes a sentence using the given cipher table
 * 
 * @param sentence array of strings to be encoded
 * @param size size of the array
 * @param table cipher table to be used for encoding
 * @return string of the encoded sentence
*/
char* encodeSentence(const char** sentence, int size, table_t* table);

#endif