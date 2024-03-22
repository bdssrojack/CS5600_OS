#include <time.h>

#include "message.h"

#define NUM_ACCESSES 1000
#define NUM_MESSAGES 100 

int main(int argc, char const* argv[]) {
    srand(time(NULL));

    cache_t* cache = get_cache_instance();

    /*Test for the cache mechanism*/

    // create the representative message set
    for(int i = 0; i < NUM_MESSAGES; i++) {
        char sender[SENDER_SIZE];
        char receiver[RECEIVER_SIZE];
        char content[CONTENT_SIZE];
        snprintf(sender, SENDER_SIZE, "sender%d", i);
        snprintf(receiver, RECEIVER_SIZE, "receiver%d", i);
        snprintf(content, CONTENT_SIZE, "content%d", i);
        message_t* msg = create_msg(sender, receiver, content);
        store_msg(msg);
    }

    // randomly access the messages
    int cache_hits = 0;
    for(int i = 0; i < NUM_ACCESSES; i++) {
        int id = rand() % NUM_MESSAGES;

        // check if the message is in the cache
        for(int j = 0; j < cache->size; j++) {
            if(cache->messages[j]->id == id) {
                cache_hits++;
                break;
            }
        }

        // retrieve the message
        message_t* msg = retrieve_msg(id);
    }

    double hit_rate = (double)cache_hits / NUM_ACCESSES;
    char* algo = LRU ? "LRU" : "Random replacement";
    printf("====================================\n");
    printf("Number of accesses: %d\n", NUM_ACCESSES);
    printf("Number of messages: %d\n", NUM_MESSAGES);
    printf("Cache hit for %s: %d\n", algo, cache_hits);
    printf("Cache miss for %s: %d\n", algo, NUM_ACCESSES - cache_hits);
    printf("Cache hit rate for %s: %.2f\n", algo, hit_rate);

    char* file_name = LRU ? "cache_results_LRU.txt" : "cache_results_Random.txt";
    FILE* file = fopen(file_name, "a");
    if(file == NULL) {
        perror("Failed to open file");
        return 1;
    }
    // Print to file
    fprintf(file, "====================================\n");
    fprintf(file, "Number of accesses: %d\n", NUM_ACCESSES);
    fprintf(file, "Number of messages: %d\n", NUM_MESSAGES);
    fprintf(file, "Cache hit for %s: %d\n", algo, cache_hits);
    fprintf(file, "Cache miss for %s: %d\n", algo, NUM_ACCESSES - cache_hits);
    fprintf(file, "Cache hit rate for %s: %.2f\n", algo, hit_rate);

    fclose(file);

    free_cache();

    return 0;
}
