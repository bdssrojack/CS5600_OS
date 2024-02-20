#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

/* Answer to question 4
   The data inconsistency is caused by the fact that the read_arr thread is reading the array while the write_arr thread is writing to it.
   One way to fix this is to use a mutex to lock the array while it is being written to, and unlock it for reading when it is done.
*/

int arr[1000];
 
void *write_arr(void *data)
{
    char *name = (char*)data;
 
    for (int i = 0; i < 120; i++)
    {
        usleep(50000);
        arr[i] = i;
        printf("%s: write %d to arr[%d]\n", name, i, i);
    }
 
    printf("Thread %s done!\n", name);
    return NULL;
}

void *read_arr(void *data)
{
    char *name = (char*)data;
 
    for (int i = 0; i < 120; i++)
    {
        usleep(50000);
        printf("%s: arr[%d] is %d\n", name, i, arr[i]);
    }
 
    printf("Thread %s done!\n", name);
    return NULL;
}
 
int main(void)
{
    pthread_t th1, th2;
    pthread_create(&th1, NULL, write_arr, "X");
    pthread_create(&th2, NULL, read_arr, "Y");
    sleep(1);
    printf("====> Cancelling Thread Y %ld\n", th1);
    pthread_cancel(th2);
    usleep(100000);
    printf("====> Cancelling Thread X %ld\n", th2);
    pthread_cancel(th1);
    printf("Exiting from main program\n");
    // pthread_join(th1, NULL);
    // pthread_join(th2, NULL);
    return 0;
}