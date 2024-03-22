/**
 * @file message.h
 * @author Zhongshi Luo
 * @brief Date structure for message, contains a unique id, timestamp, sender,
 * receiver, message content and a flag to indicate if the message was delivered
 * @version 0.1
 * @date 2024-03-19
 *
 * @copyright Copyright (c) 2024
 *
 */

#ifndef MESSAGE_H
#define MESSAGE_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

#include "cache.h"

#define MESSAGE_SIZE 512
#define SENDER_SIZE 100
#define RECEIVER_SIZE 100
#define CONTENT_SIZE 256

#ifndef LRU
#define LRU 0
#endif

typedef struct Message {
    int id;
    unsigned int timestamp;
    char sender[SENDER_SIZE];
    char receiver[RECEIVER_SIZE];
    char content[CONTENT_SIZE];
    bool delivered;
} message_t;

/**
 * @brief Create a message object
 *
 * @return message_t*: pointer of dynamically allocated message object
 */
message_t* create_msg(const char* sender, const char* receiver,
                      const char* content);

/**
 * @brief Store the message in the *message store* on disk and cache
 *
 * @param msg message object
 * @return true if the message was stored successfully
 * @return false if the message was not stored successfully
 */
bool store_msg(const message_t* msg);

/**
 * @brief Retrieve a message by its unique id from the cache and message store
 *
 * @param id unique id of the message
 * @return message_t* pointer of the message object found
 */
message_t* retrieve_msg(const int id);

#endif