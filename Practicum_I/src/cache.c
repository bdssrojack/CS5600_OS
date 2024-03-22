#include "cache.h"

cache_t* get_cache_instance() {
    // singleton instance, only called on the first time
    static cache_t* cache = NULL;

    if(cache == NULL) {
        cache = (cache_t*)malloc(sizeof(cache_t));
        if(cache == NULL) {
            perror("Failed to allocate memory for cache");
            return NULL;
        }
        cache->size = 0;
    }

    return cache;
}

void free_cache() {
    cache_t* cache = get_cache_instance();
    if(cache != NULL) {
        for(int i = 0; i < cache->size; i++) {
            free(cache->messages[i]);
        }
        free(cache);
    }
}
