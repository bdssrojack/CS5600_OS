#include "message.h"

int main(int argc, char const* argv[]) {
    cache_t* cache = get_cache_instance();

    message_t* msg = create_msg("sender", "receiver", "content");
    printf("size of message_t: %lu\n", sizeof(message_t));

    store_msg(msg);

    message_t* m = retrieve_msg(1);

    free_cache();

    return 0;
}
