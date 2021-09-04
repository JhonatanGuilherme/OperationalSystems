#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

const clock_t MAXDELAY = 2000000;
int x = 50, y = 50;

void delay(clock_t ticks);
void * adjustX(void *n);

int main(void) {
  int a;
  srand(time(NULL));

  pthread_t up_thread, dn_thread;
  pthread_attr_t *attr;attr = 0;

  printf("creating threads:\n");
  pthread_create(&up_thread, attr, adjustX, (void *)1);
  pthread_create(&dn_thread, attr, adjustX, (void *)-1);
  
  while (1) { ; }

  return 0;
}

void delay(clock_t ticks) {
clock_t start = clock();
do
; while (clock() < start + ticks);
}

void * adjustX(void *n) {
  int i = (int)n;

  while (1) {
    if (i == 1) {
      printf("adjustment = %2i; x = %i\n", i, x);
      x += i;
    } else {
      printf("adjustment = %2i; y = %i\n", i, y);
      y += i;
    }
    delay(rand() % MAXDELAY);
  }

  return (n);
}
