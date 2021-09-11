#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define FILENAME "processes.txt"
#define QUANTUM 2
#define SIZE 6

struct Process {
  char name[SIZE];
  int time;
  struct Process *next;
};

struct Process *first = NULL, *last = NULL;

void insertProcess(struct Process process);
struct Process *deleteProcess(struct Process *process);

int main(void) {
  FILE *file = fopen(FILENAME, "r"); 

  if (!file) {
    printf("Could not open file!\n");
    exit(-1);
  }

  char line[SIZE];
  struct Process *process = NULL;
  
  while (!feof(file)) {
    process = (struct Process*) malloc(sizeof(struct Process));

    char *result = fgets(line, SIZE, file);
    char *token = strtok(result, " ;");

    while (token) {
      int time = atoi(token);
      if (!time)
        strcpy(process->name, token);
      else {
        process->time = time;
        printf("Process %s is added.\n", process->name);
        insertProcess(*process);
      }
      token = strtok(NULL, " ;\n");        
    }
  }

  fclose(file);

  printf("\n");

  process = first;
  
  int count = 0;
  while (1) {
    printf("Process %s is running.\n", process->name);
    sleep(QUANTUM);
    process->time -= QUANTUM;
    if (process->time < 0) {
      printf("Process %s completed.\n", process->name);
      process = deleteProcess(process);
    } else {
      process = process->next;
    }
    if (!first)
      break;
    if (!process)
      process = first;
  }

  return 0;
}

void insertProcess(struct Process process) {
  struct Process *node = (struct Process*) malloc(sizeof(struct Process));

  if (!node) {
    printf("Could not allocate memory!\n");
    exit(1);
  }

  *node = process;

  if (!first) {
    node->next = NULL;
    first = last = node;
  } else {
    node->next = NULL;
    last->next = node;
    last = node;
  }

  return;
}

struct Process *deleteProcess(struct Process *process) {
  if (first == process) {
    first = first->next;
    free(process);
    return first;
  }

  struct Process *node = first;

  while (node) {
    if (node->next == process) {
      node->next = process->next;
      return node->next;
    }
    node = node->next;
  }

  return process;
}
