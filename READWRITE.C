#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

sem_t rw_mutex, mutex;
int read_count = 0;

void *reader(void *arg) {
    sem_wait(&mutex);
    read_count++;
    if (read_count == 1)
        sem_wait(&rw_mutex);
    sem_post(&mutex);
    
    printf("Reader is reading\n");
    sleep(1);
    
    sem_wait(&mutex);
    read_count--;
    if (read_count == 0)
        sem_post(&rw_mutex);
    sem_post(&mutex);
}

void *writer(void *arg) {
    sem_wait(&rw_mutex);
    printf("Writer is writing\n");
    sleep(2);
    sem_post(&rw_mutex);
}

int main() {
    pthread_t r1, r2, w1;
    sem_init(&rw_mutex, 0, 1);
    sem_init(&mutex, 0, 1);

    pthread_create(&r1, NULL, reader, NULL);
    pthread_create(&w1, NULL, writer, NULL);
    pthread_create(&r2, NULL, reader, NULL);
    
    pthread_join(r1, NULL);
    pthread_join(w1, NULL);
    pthread_join(r2, NULL);
    
    sem_destroy(&rw_mutex);
    sem_destroy(&mutex);
    return 0;
}


ORR 

#include <pthread.h>
#include <stdio.h>

typedef int semaphore;
semaphore mutex = 1;
semaphore db = 1;
int rc = 0;
int total_cycles = 5;
int max_iterations = 3;

void down(semaphore* s) {
    while (*s <= 0);
    (*s)--;
}

void up(semaphore* s) {
    (*s)++;
}

void* reader(void* arg) {
    for (int i = 0; i < max_iterations; i++) {
        down(&mutex);
        rc++;
        if (rc == 1) down(&db);
        up(&mutex);

        printf("Reader is reading...\n");

        down(&mutex);
        rc--;
        if (rc == 0) up(&db);
        up(&mutex);
    }
    pthread_exit(NULL);
}
void* writer(void* arg) {
    for (int i = 0; i < max_iterations; i++) {
        down(&db);
        printf("Writer is writing...\n");
        up(&db);
    }
pthread_exit(NULL);
}

int main() {
    pthread_t reader_thread, writer_thread;

    for (int i = 0; i < total_cycles; i++) {
        pthread_create(&reader_thread, NULL, reader, NULL);
        pthread_join(reader_thread, NULL);

        pthread_create(&writer_thread, NULL, writer, NULL);
        pthread_join(writer_thread, NULL);
    }

    return 0;
}

