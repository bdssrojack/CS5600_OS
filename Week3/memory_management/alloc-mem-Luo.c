#include <stdlib.h>

#define MEGABYTE 1024U*1024U

/* static memory is assigned to a global or static variable when declaring. It has the extent with the life of the program.
*/
char static_mem[5 * MEGABYTE];


void func() {
    /* The local variable will be allocated on stack memory when gets called
    */
    char stack_mem[MEGABYTE];
    /* Stack memory deallocated when function returns
    */
}

int main() {
    /* Heap memory is allocated by malloc(). It keeps existing until free() is called on it.
    * if free() is not called before return, it will remain in the heap and cause memory leak
    */
    char* heap_mem = (char*)malloc((size_t)10 * MEGABYTE);
    if(heap_mem != NULL)
    /*Here the heap memory is recycled*/
        free(heap_mem);

    /* Call the function to allocate stack memory*/
    func();

    /*Here the static memory and stack memory is recycled*/
    return 0;
}