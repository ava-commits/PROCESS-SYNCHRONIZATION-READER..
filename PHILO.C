3. Dining Philosopher Problem
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define NUM_PHILOSOPHERS 5

sem_t forks[NUM_PHILOSOPHERS]; 
sem_t mutex;                   

void* philosopher(void* num) {
    int index = *(int*)num;
    
    while (1) {
        printf("Philosopher %d is thinking...\n", index);
        sleep(rand() % 5 + 1); 

        sem_wait(&mutex); 
        
        sem_wait(&forks[index]); 
        sem_wait(&forks[(index + 1) % NUM_PHILOSOPHERS]); 

        sem_post(&mutex); 

        printf("Philosopher %d is eating...\n", index);
        sleep(rand() % 5 + 1); 

        sem_post(&forks[index]); 
        sem_post(&forks[(index + 1) % NUM_PHILOSOPHERS]); 
    }
}

int main() {
    pthread_t philosophers[NUM_PHILOSOPHERS];
    int indices[NUM_PHILOSOPHERS];

    sem_init(&mutex, 0, 1); 
    
    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        sem_init(&forks[i], 0, 1); 
    }

    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        indices[i] = i;
        pthread_create(&philosophers[i], NULL, philosopher, &indices[i]);
    }

    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        pthread_join(philosophers[i], NULL);
    }

    return 0;
}

ORR

#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <unistd.h>
#define N 5
#define left (i+N-1)%N
#define right (i+1)%N
#define thinking 0
#define hungry 1
#define eating 2
int state[N];
sem_t mutex;
sem_t s[N];
void print_states() {
printf("States: ");
for (int j = 0; j < N; j++) {
if (state[j] == thinking)
printf("T ");
else if (state[j] == hungry)
printf("H ");
else if (state[j] == eating)
printf("E ");
}
printf("\n");
}
void test(int i) {
if (state[i] == hungry && state[left] != eating && state[right] != eating) {
state[i] = eating;
sem_post(&s[i]);
}
}
void take_fork(int i) {
sem_wait(&mutex);
state[i] = hungry;
printf("Philosopher %d is Hungry\n", i + 1);
print_states();
test(i);
sem_post(&mutex);
sem_wait(&s[i]);
}
void put_fork(int i) {
sem_wait(&mutex);
state[i] = thinking;
printf("Philosopher %d is Thinking\n", i + 1);
print_states();
test(left);
test(right);
sem_post(&mutex);
}
void think() {
sleep(1);
}
void eat() {
sleep(1);
}
void* philosopher(void* num) {
int i = (int)num;
while (1) {
think();
take_fork(i);
printf("Philosopher %d is Eating\n", i + 1);
print_states();
eat();
put_fork(i);
}
}
int main() {
int i;
pthread_t thread_id[N];
int phil[N];
sem_init(&mutex, 0, 1);
for (i = 0; i < N; i++) {
sem_init(&s[i], 0, 0);
state[i] = thinking;
phil[i] = i;
}
for (i = 0; i < N; i++) {
pthread_create(&thread_id[i], NULL, philosopher, &phil[i]);
printf("Philosopher %d is Thinking\n", i + 1);
print_states();
}
for (i = 0; i < N; i++) {
pthread_join(thread_id[i], NULL);
}
return 0;
}
