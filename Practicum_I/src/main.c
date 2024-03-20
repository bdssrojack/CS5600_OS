#include "message.h"

int main(int argc, char const *argv[])
{
    message_t* msg = create_msg("sender", "receiver", "content");
    store_msg(msg);
    retrieve_msg(1);
    return 0;
}
