

#ifndef QUEUE_H
#define QUEUE_H
#include "process.h"
//#include "kernel.h"

 
 
#define ERR_OK      0
#define ERR_NORES   1
#define ERR_PARAMS    2

typedef struct {
   procent  *first;
   procent  *last;
} queue_t;

typedef struct {
  uint32 value;
  queue_t waitq;
  
}sem_t;

uint32 sem_init(sem_t * sem, uint32 value);
uint32 sem_signal(sem_t * sem);
uint32 sem_signaln(sem_t *sem,int);
uint32 sem_wait(sem_t * sem);
uint32 sem_count(sem_t * sem);

void queue_init(queue_t *q);

void queue_push(queue_t *q,  procent  *t);

void queue_pushsort(queue_t *q,  procent  *t);

 procent * queue_pop(queue_t *q);
 procent * queue_peek(queue_t *q);


extern queue_t ready_queue;
extern queue_t sleep_queue;
extern sem_t tx;



#endif