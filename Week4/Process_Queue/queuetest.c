#include "queue.h"

int main(int argc, char const *argv[]) {
    queue_t* queue = createQ();

    process_t* p1 = createProcess(1, "p1", 100, 1);
    add2q(queue, p1);
    printQ(queue);

    process_t* p2 = createProcess(2, "p2", 200, 2);
    add2q(queue, p2);
    printQ(queue);

    process_t* p3 = createProcess(3, "p3", 300, 3);
    add2q(queue, p3);
    printQ(queue);

    process_t* p4 = createProcess(4, "p4", 400, 4);
    add2q(queue, p4);
    printQ(queue);

    process_t* p5 = popQ(queue);
    printQ(queue);

    process_t* p6 = rmProcess(queue);
    printQ(queue);

    deleteQ(queue);
    return 0;
}
