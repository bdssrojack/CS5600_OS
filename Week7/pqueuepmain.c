#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define BUF_SIZE 15
/* number of producers and consumers */
#define NUM_PRODUCERS 2
#define NUM_CONSUMERS 2
/* When a total of 30 items have been produced or consumed, print a message and
 * exit all the producer/consumer threads. */
#define MAX_ITEMS 30

int buffer[BUF_SIZE];
int num = 0;
/* count the item consumed/produced */
int consumed_item_cnt = 0;
int produced_item_cnt = 0;
pthread_mutex_t mut;
pthread_cond_t cond;

void *producer(void* param);
void *consumer(void* param);

int main(int argc, char *argv[]) {

  printf("Initialising the mutex\n");
  if (pthread_mutex_init(&mut, NULL) != 0) {
    perror("pthread_mutex_init");
    exit(1);
  }

  printf("Initialising the condition variable\n");
  if (pthread_cond_init(&cond, NULL) != 0) {
    perror("pthread_cond_init");
    exit(1);
  }

  printf("Creating the producer thread\n");
  pthread_t producers[NUM_PRODUCERS];
  for (int i = 0; i < NUM_PRODUCERS; i++) {
    if (pthread_create(&producers[i], NULL, producer, (void*)(__intptr_t)i) != 0) {
      perror("pthread_create");
      exit(1);
    }
  }

  printf("Creating the consumer thread\n");
  pthread_t consumers[NUM_CONSUMERS];
  for(int i = 0; i < NUM_CONSUMERS; i++) {
    if (pthread_create(&consumers[i], NULL, consumer, (void*)(__intptr_t)i) != 0) {
      perror("pthread_create");
      exit(1);
    }
  }

  for(int i = 0; i < NUM_PRODUCERS; i++) {
    pthread_join(producers[i], NULL);
    pthread_join(consumers[i], NULL);
  }

  printf("Main: All threads have finished.\n");
}

void *producer(void* param) {
  int idx = (__intptr_t)param;
  int i = 0;
  while (1) {
    pthread_mutex_lock(&mut);
    printf("Producer %d: mutex locked\n", idx);

    if (produced_item_cnt >= MAX_ITEMS) {
      printf("Producer %d: exit for produced_item_cnt >= MAX_ITEMS\n", idx);
      pthread_mutex_unlock(&mut);
      pthread_exit(NULL);
    }

    if (num > BUF_SIZE) {
      printf("Producer %d: exit for num > BUF_SIZE\n", idx);
      exit(1);
    }

    while (num == BUF_SIZE) {
      printf("Producer %d: waiting for enough space\n", idx);
      pthread_cond_wait(&cond, &mut);
    }

    buffer[num] = i;
    num++;
    produced_item_cnt++;
    printf("Producer %d: producing item at block %d, produced count = %d\n", idx, num, produced_item_cnt);

    pthread_mutex_unlock(&mut);
    pthread_cond_signal(&cond);

    printf("Producer %d: unlock and signal\n", idx);
    sleep(0.5);
  }
}

void *consumer(void* param) {
  int idx = (__intptr_t)param;
  int i = 0;
  while (1) {
    pthread_mutex_lock(&mut);
    printf("Consumer %d: mutex locked\n", idx);

    if (consumed_item_cnt >= MAX_ITEMS) {
      printf("Consumer %d: exit for consumed_item_cnt >= MAX_ITEMS\n", idx);
      pthread_mutex_unlock(&mut);
      pthread_exit(NULL);
    }

    if (num < 0) {
      printf("Consumer %d: exit for negative num\n", idx);
      exit(1);
    }

    while (num == 0) {
      printf("Consumer %d: waiting for enough items\n", idx);
      pthread_cond_wait(&cond, &mut);
    }

    i = buffer[num];
    consumed_item_cnt++;
    printf("Consumer %d: polling %d, num = %d, consumed count = %d\n", idx, i, num, consumed_item_cnt);
    num--;

    pthread_mutex_unlock(&mut);
    pthread_cond_signal(&cond);
    printf("Consumer %d: unlock and signal\n", idx);
    sleep(1);
  }
}
