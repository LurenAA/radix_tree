#include "simpleQueue.h"

simpleQueue* simpleQueueFactory(int number){
  int size = sizeof(simpleQueue) + sizeof(void *) * number;
  simpleQueue* queue = (simpleQueue*)malloc(size);
  if(!queue) return NULL;
  queue->start = queue->end = -1;
  queue->capacity = number;
  return queue;
}

void pop(simpleQueue* queue){
  if(isEmpty(queue)) return;
  queue->start = (queue->start + 1 ) % queue->capacity;
  if(queue->start == queue->end) 
    queue->start = queue->end = -1;
}

int push(simpleQueue* queue, void *val) {
  if(isFull(queue)) return -1;
  if(queue->end == -1 && queue->start == -1) {
    queue->start = 0;
    queue->data[0] = val;
    queue->end = 1;
    return 0;
  }
  int index = queue->end;
  queue->end = (queue->end + 1) %  queue->capacity;
  queue->data[index] = val;
  return index;
}

bool isFull(simpleQueue* queue) {
  return queue->end == queue->start && queue->end != -1;
}

bool isEmpty(simpleQueue* queue){
  return queue->end == queue->start && queue->end == -1;
}

int size(simpleQueue * queue) {
  if(isEmpty(queue)) return 0;
  return queue->end <= queue->start ? 
    queue->end - queue->start + queue->capacity :
    queue->end - queue->start;
}

void* front(simpleQueue* queue){
  if(isEmpty(queue)) return NULL;
  return queue->data[queue->start];
}

void simpleQueueDebug(char ** s,int number){
  fprintf(stderr, "test simpleQueue: \n");
  bool flag = true;
  simpleQueue* qu = simpleQueueFactory(number);
  if(!qu) {
    fprintf(stderr, "malloc: %s\n", strerror(errno));
  }
  fprintf(stdout, "%d:%d:%d\n", isFull(qu), isEmpty(qu), size(qu));
  start: for(int i = 0; i < number; i++) {
    if(push(qu, s[i]) < 0)
      fprintf(stderr, "push error\n");
    fprintf(stdout, "%d:%d:%d\n", isFull(qu), isEmpty(qu), size(qu));
  }
  if(push(qu, s[0]) == -1)
      fprintf(stderr, "full\n");
  for(int i = 0; i < number; i++) {
    fprintf(stdout, "%d:%d:%d:%s\n", isFull(qu), isEmpty(qu), size(qu), (char*)front(qu));
    pop(qu);
    if(i == 3 && flag) {
      flag = false;
      goto start;
    }    
  }
  fprintf(stdout, "%d:%d:%d\n", isFull(qu), isEmpty(qu), size(qu));
  free(qu);
  fprintf(stderr, "test end\n");
}
