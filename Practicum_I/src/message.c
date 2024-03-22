#include "message.h"

/* static id counter */
static int id_counter = 0;

/**
 * @brief Generate the file path for the message
 *
 * @param id unique id of the message
 * @param filePath buffer to store the file path
 * @param size size of the buffer
 */
void msg_file_path(int id, char *filePath, size_t size) {
    snprintf(filePath, size, "msg_store/msg-%d.txt", id);
}

/**
 * @brief Free the message object
 *
 * @param msg pointer of the message object
 */
void free_msg(message_t *msg) {
    if (msg != NULL) {
        free(msg);
    }
}

void print_cache() {
    cache_t *cache = get_cache_instance();
    printf("--------------------\n");
    printf("Cache size: %d.\n", cache->size);
    for (int i = 0; i < cache->size; i++) {
        printf("Cache[%d]: id=%d, sender=%s, receiver=%s, content=%s\n", i,
               cache->messages[i]->id, cache->messages[i]->sender,
               cache->messages[i]->receiver, cache->messages[i]->content);
    }
    printf("--------------------\n");
}

void add_to_cache(message_t *msg) {
    cache_t *cache = get_cache_instance();

#if LRU
    // LRU: if the cache is full, remove the oldest message
    if (cache->size == MAX_CACHE_SIZE) {
        free_msg(cache->messages[0]);
        memmove(&cache->messages[0], &cache->messages[1],
                (cache->size - 1) * sizeof(message_t *));
        cache->size--;
    }
#else
    // Random replacement: generate a random number in the range of the cache
    // size
    if (cache->size == MAX_CACHE_SIZE) {
        int index = rand() % MAX_CACHE_SIZE;
        free_msg(cache->messages[index]);
        cache->messages[index] = msg;
        return;
    }
#endif

    // add the message to the end of the cache
    cache->messages[cache->size++] = msg;

    printf(
        "Message added to cache: id=%d, sender=%s, receiver=%s, content=%s\n",
        msg->id, msg->sender, msg->receiver, msg->content);
    // print cache
    print_cache();
}

message_t *create_msg(const char *sender, const char *receiver,
                      const char *content) {
    message_t *msg = (message_t *)malloc(sizeof(message_t));

    msg->id = ++id_counter;

    strncpy(msg->sender, sender, SENDER_SIZE - 1);
    msg->sender[SENDER_SIZE - 1] = '\0';

    strncpy(msg->receiver, receiver, RECEIVER_SIZE - 1);
    msg->receiver[RECEIVER_SIZE - 1] = '\0';

    strncpy(msg->content, content, CONTENT_SIZE - 1);
    msg->content[CONTENT_SIZE - 1] = '\0';

    msg->delivered = false;

    // set the id and ts with the current time in microseconds
    struct timeval tv;
    gettimeofday(&tv, NULL);
    msg->timestamp = (unsigned int)(tv.tv_usec);

    printf("Message created: id=%d, sender=%s, receiver=%s, content=%s\n",
           msg->id, msg->sender, msg->receiver, msg->content);

    return msg;
}

bool store_msg(const message_t *msg) {
    if (msg == NULL) {
        return false;
    }

    // Check if the directory exists, if not create it
    struct stat st = {0};
    if (stat("msg_store", &st) == -1) mkdir("msg_store", 0755);

    char filePath[256];
    msg_file_path(msg->id, filePath, sizeof(filePath));

    // Open the file for writing
    FILE *file = fopen(filePath, "a");
    if (file == NULL) {
        perror("open");
        return false;
    }

    // Write the message to the file
    fprintf(file, "%d,%u,%s,%s,%s,%d\n", msg->id, msg->timestamp, msg->sender,
            msg->receiver, msg->content, msg->delivered);
    fclose(file);

    // TODO: whether to free the message here or not

    // add the message to the cache
    add_to_cache((message_t *)msg);

    return true;
}

message_t *retrieve_msg(const int id) {
    // retrieve the message from the cache first
    cache_t *cache = get_cache_instance();
    for (int i = 0; i < cache->size; i++) {
        if (cache->messages[i]->id == id) {
// LRU: move the message to the end of the cache array
#if LRU
            if (i != cache->size - 1) {
                message_t *temp = cache->messages[i];
                memmove(&cache->messages[i], &cache->messages[i + 1],
                        (cache->size - i - 1) * sizeof(message_t *));
                cache->messages[cache->size - 1] = temp;
            }
#endif
            printf(
                "Message retrieved from cache: id=%d, sender=%s, receiver=%s, "
                "content=%s\n",
                cache->messages[i]->id, cache->messages[i]->sender,
                cache->messages[i]->receiver, cache->messages[i]->content);
            print_cache();
            return cache->messages[i];
        }
    }

    // if the message is not found in the cache, retrieve it from the message
    // store
    char filePath[256];
    msg_file_path(id, filePath, sizeof(filePath));

    FILE *file = fopen(filePath, "r");
    if (file == NULL) {
        perror("open");
        return NULL;
    }

    // Get the first line as message from the file
    char line[MESSAGE_SIZE];
    if (fgets(line, sizeof(line), file) == NULL) {
        perror("fgets");
        return NULL;
    }

    message_t *msg = (message_t *)malloc(sizeof(message_t));
    if (msg == NULL) {
        perror("malloc");
        return NULL;
    }

    // Parse the line and store the message
    char *token = strtok(line, ",");
    if (token != NULL) msg->id = atoi(token);

    token = strtok(NULL, ",");
    if (token != NULL) msg->timestamp = (unsigned int)strtoul(token, NULL, 10);

    token = strtok(NULL, ",");
    if (token != NULL) {
        strncpy(msg->sender, token, SENDER_SIZE - 1);
        msg->sender[SENDER_SIZE - 1] = '\0';
    }

    token = strtok(NULL, ",");
    if (token != NULL) {
        strncpy(msg->receiver, token, RECEIVER_SIZE - 1);
        msg->receiver[RECEIVER_SIZE - 1] = '\0';
    }

    token = strtok(NULL, ",");
    if (token != NULL) {
        strncpy(msg->content, token, CONTENT_SIZE - 1);
        msg->content[CONTENT_SIZE - 1] = '\0';
    }

    token = strtok(NULL, ",");
    if (token != NULL) msg->delivered = (bool)atoi(token);

    fclose(file);

    // add the message to the cache
    add_to_cache(msg);

    printf(
        "Message retrieved from store: id=%d, sender=%s, receiver=%s, "
        "content=%s\n",
        msg->id, msg->sender, msg->receiver, msg->content);

    return msg;
}