#include "message.h"

static int id_counter = 0;

void msg_file_path(int id, char* filePath, size_t size) {
    snprintf(filePath, size, "msg_store/msg-%d.txt", id);
}

void free_msg(message_t* msg) {
    if(msg != NULL) {
        free(msg->sender);
        free(msg->receiver);
        free(msg->content);
        free(msg);
    }
}

message_t* create_msg(char* sender, char* receiver, char* content) {
    message_t* msg = (message_t*)malloc(sizeof(message_t));

    msg->id = ++id_counter;
    msg->sender = sender;
    msg->receiver = receiver;
    msg->content = content;
    msg->delivered = false;

    // set the id and ts with the current time in microseconds
    struct timeval tv;
    gettimeofday(&tv, NULL);
    msg->timestamp = (unsigned int)(tv.tv_usec);
    
    return msg;
}

bool store_msg(message_t* msg) {
    if(msg == NULL) {
        return false;
    }

    mkdir("msg_store", 0755);

    char filePath[256];
    msg_file_path(msg->id, filePath, sizeof(filePath));

    // Open the file for writing
    FILE* file = fopen(filePath, "a");
    if (file == NULL) {
        perror("open");
        return false;
    }

    // Write the message to the file
    fprintf(file, "%d,%u,%s,%s,%s,%d",
            msg->id, msg->timestamp, msg->sender, msg->receiver, msg->content, msg->delivered);
    fclose(file);

    // TODO: whether to free the message here or not

    return true;
}


message_t* retrieve_msg(int id) {
    char filePath[256];
    msg_file_path(id, filePath, sizeof(filePath));

    FILE* file = fopen(filePath, "r");
    if (file == NULL) {
        perror("open");
        return NULL;
    }

    char line[MESSAGE_SIZE];
    if (fgets(line, sizeof(line), file) == NULL) {
        perror("fgets");
        return NULL;
    }
    
    message_t* msg = (message_t*)malloc(sizeof(message_t));
    if (msg == NULL) {
        perror("malloc");
        return NULL;
    }

    char* token = strtok(line, ",");
    if (token != NULL) 
        msg->id = atoi(token);

    token = strtok(NULL, ",");
    if (token != NULL) 
        msg->timestamp = (unsigned int)strtoul(token, NULL, 10);

    token = strtok(NULL, ",");
    if (token != NULL) 
        msg->sender = strdup(token);

    token = strtok(NULL, ",");
    if (token != NULL) 
        msg->receiver = strdup(token);

    token = strtok(NULL, ",");
    if (token != NULL) 
        msg->content = strdup(token);

    token = strtok(NULL, ",");
    if (token != NULL) 
        msg->delivered = (bool)atoi(token);

    fclose(file);
    return msg;
}