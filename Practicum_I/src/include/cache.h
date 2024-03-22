/**
 * @file cache.h
 * @author Zhongshi Luo
 * @brief The cache is a fixed size array of message_t pointers. The size of the cache is defined by the constant MAX_CACHE_SIZE. 
 *        Every message stored is stored in both the cache and the message store. 
 *        When the cache is full, the oldest message is removed to make space for the new message using LRU.
 *        When a message is retrieved, the cache is checked first. If not found, the message needs to be loaded from disk and place in cache.
 * @version 0.1
 * @date 2024-03-20
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef CACHE_H
#define CACHE_H

#include <stdlib.h>
#include <stdio.h>

// forward declaration of cache_t
typedef struct Message message_t;

#define MAX_CACHE_SIZE 16

typedef struct Cache {
	int size;
	message_t* messages[MAX_CACHE_SIZE];
} cache_t;

/*
Reason for choosing array as the cache: The efficeincy of random access is a good fit for random replacement algorithm; 
also can handle the LRU replacement algorithm in a simple modification;
the simplicity of implementation is also a good fit for the simple cache system.

strategy and design: For random access page replacement, generate a random number in the range of the cache size, then replace the message in that index.
For LRU page replacement, once the message is accessed(i.e. inserted or retrieved), move the message to the end of the cache array. When the cache is full,
remove the message at the beginning of the cache array. The messages in the cache array are rearranged using memmove()

Another two alternative designs are linked list and hash table. The reason they are abandoned is that linked list is not efficient for random access, 
and hash table is way too more complicated to implement for this simple cache system.
*/


/**
 * @brief Get the cache instance object as a singleton
 * 
 * @return cache_t* 
 */
cache_t* get_cache_instance();

/**
 * @brief Free the cache instance object and message objects in the cache
 * 
 */
void free_cache();


#endif
