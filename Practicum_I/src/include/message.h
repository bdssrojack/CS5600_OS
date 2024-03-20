/**
 * @file message.h
 * @author Zhongshi Luo
 * @brief Date structure for message, contains a unique id, timestamp, sender, receiver, message content and a flag to indicate if the message was delivered
 * @version 0.1
 * @date 2024-03-19
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef MESSAGE_H
#define MESSAGE_H

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#define MESSAGE_SIZE 1024

typedef struct Message
{
    int id;
    unsigned int timestamp;
    char* sender;
    char* receiver;
    char* content;
    bool delivered;
}message_t;

/**
 * @brief Create a message object
 * 
 * @return message_t*: pointer of dynamically allocated message object
 */
message_t* create_msg(char* sender, char* receiver, char* content);

/**
 * @brief Store the message in the *message store* on disk
 * 
 * @param msg message object
 * @return true if the message was stored successfully
 * @return false if the message was not stored successfully
 */
bool store_msg(message_t* msg);

/**
 * @brief Retrieve a message by its unique id from the *message store*
 * 
 * @param id unique id of the message
 * @return message_t* pointer of the message object found
 */
message_t* retrieve_msg(int id);

#endif