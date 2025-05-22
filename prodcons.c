1. Producer Consumer Problem

#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define BUFFER_SIZE 5  
#define TOTAL_ITEMS 10 

int buffer[BUFFER_SIZE];
int in = 0, out = 0, count = 0; 

sem_t empty, full; 
pthread_mutex_t mutex; 

void* producer(void* arg) {
    for (int i = 0; i < TOTAL_ITEMS; i++) {
        int item = rand() % 100; 

        sem_wait(&empty); 
        pthread_mutex_lock(&mutex); 

        buffer[in] = item;
        printf("Producer produced: %d\n", item);
        in = (in + 1) % BUFFER_SIZE; 
        count++;

        pthread_mutex_unlock(&mutex); 
        sem_post(&full); 

        sleep(rand() % 2 + 1); 
    }
    pthread_exit(NULL);
}

void* consumer(void* arg) {
    for (int i = 0; i < TOTAL_ITEMS; i++) {
        sem_wait(&full); 
        pthread_mutex_lock(&mutex); 

        int item = buffer[out];
        printf("Consumer consumed: %d\n", item);
        out = (out + 1) % BUFFER_SIZE; 
        count--;

        pthread_mutex_unlock(&mutex); 
        sem_post(&empty); 

        sleep(rand() % 2 + 1); 
    }
    pthread_exit(NULL);
}

int main() {
    pthread_t producer_thread, consumer_thread;

    sem_init(&empty, 0, BUFFER_SIZE); 
    sem_init(&full, 0, 0); 
    pthread_mutex_init(&mutex, NULL); 

    pthread_create(&producer_thread, NULL, producer, NULL);
    pthread_create(&consumer_thread, NULL, consumer, NULL);

    pthread_join(producer_thread, NULL);
    pthread_join(consumer_thread, NULL);

    sem_destroy(&empty);
    sem_destroy(&full);
    pthread_mutex_destroy(&mutex);

    return 0;
}


orr producer consumer
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define BUFFER_SIZE 5
#define MAX_ITEMS 5

int buffer[BUFFER_SIZE];
int in = 0;
int out = 0;
int produced_count = 0;
int consumed_count = 0;

pthread_mutex_t mutex;
pthread_cond_t full;
pthread_cond_t empty;

void* producer(void* arg) {
   int item = 1;

   while (produced_count < MAX_ITEMS) {
      pthread_mutex_lock(&mutex);

      while (((in + 1) % BUFFER_SIZE) == out) {
         pthread_cond_wait(&empty, &mutex);
      }

      buffer[in] = item;
      printf("Produced: %d", item);
      item++;
      in = (in + 1) % BUFFER_SIZE;

      produced_count++;

      pthread_cond_signal(&full);
      pthread_mutex_unlock(&mutex);
   }

   pthread_exit(NULL);
}

void* consumer(void* arg) {
   while (consumed_count < MAX_ITEMS) {
      pthread_mutex_lock(&mutex);

      while (in == out) {
         pthread_cond_wait(&full, &mutex);
      }

      int item = buffer[out];
      printf("Consumed: %d", item);
      out = (out + 1) % BUFFER_SIZE;

      consumed_count++;

      pthread_cond_signal(&empty);
      pthread_mutex_unlock(&mutex);
   }

   pthread_exit(NULL);
}

int main() {
   pthread_t producerThread, consumerThread;

   pthread_mutex_init(&mutex, NULL);
   pthread_cond_init(&full, NULL);
   pthread_cond_init(&empty, NULL);

   pthread_create(&producerThread, NULL, producer, NULL);
   pthread_create(&consumerThread, NULL, consumer, NULL);

   pthread_join(producerThread, NULL);
   pthread_join(consumerThread, NULL);

   pthread_mutex_destroy(&mutex);
   pthread_cond_destroy(&full);
   pthread_cond_destroy(&empty);

   return 0;
}

