#ifndef __SIMPLE_STACK_H_
#define __SIMPLE_STACK_H_
#include <unistd.h>
#include <malloc.h>
#include <stdbool.h>
#include <string.h>
#include <errno.h>

typedef struct 
{
  int capacity;
  int start, end;
  void* data[];
}simpleQueue;

simpleQueue* simpleQueueFactory(int);
int push(simpleQueue* ,void *val);
void pop(simpleQueue* );
bool isFull(simpleQueue* );
bool isEmpty(simpleQueue* );
int size(simpleQueue *);
void* front(simpleQueue*);
//for string
void simpleQueueDebug(char **,int number);

#endif