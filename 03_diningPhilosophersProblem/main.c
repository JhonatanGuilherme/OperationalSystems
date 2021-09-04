#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>

#define N 5
#define MAX 15000
#define LEFT (i + N - 1) % N
#define RIGHT (i + 1) % N
#define THINKING 0
#define HUNGRY 1
#define EATING 2

const clock_t MAXDELAY = 200000;
int state[6];

sem_t mutex, s[5];

pthread_t *philosophers;

void delay(void);
void *philosopher(void *arg);
void put_forks(int i);
void take_forks(int i);
void test(int i);

int main(int argc, char **argv) {
  printf("Inicializing.\n");

  pthread_t philosophers[5];
  sem_init(&mutex, 0, 1);

  for (int i = 0; i < N + 1; i++)
    state[i] = THINKING;
  
  for (int i = 0; i < N + 1; i++)
    sem_init(&s[i], 0, 0);

  for (int i = 0; i < N; i++)
    pthread_create(&philosophers[i], NULL, philosopher, &i);

  for (int i = 0; i < N; i++)
    pthread_join(philosophers[i], NULL);
  printf("Finishing.\n");
  
  return 0;
}

void delay(void) {
  srand(time(NULL));
  usleep(rand() / MAX);
}

void *philosopher(void *arg) {
  int i = *(int *) arg;
  
  while (1) {
    delay();
    take_forks(i);
    delay();
    put_forks(i);
  }
}

void put_forks(int i) {
  sem_wait(&mutex);
  state[i] = THINKING;
  printf("Philosopher %d is thinking.\n", i);
  test(LEFT);
  test(RIGHT);
  sem_post(&mutex);
}

void take_forks(int i) {
  sem_wait(&mutex);
  state[i] = HUNGRY;
  printf("Philosopher %d is hungry.\n", i);
  test(i);
  sem_post(&mutex);
  sem_wait(&s[i]);
}

void test(int i) {
  if (state[i] == HUNGRY && state[LEFT] != EATING && state[RIGHT] != EATING) {
    state[i] = EATING;
    printf("Philosopher %d is eating.\n", i);
    sem_post(&s[i]);
  }
}
