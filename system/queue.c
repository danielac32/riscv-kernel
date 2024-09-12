/* queue.c - enqueue, dequeue */

#include <xinu.h>

 void queue_init(queue_t *q) {
  q->first = 0;
  q->last = 0;
}

 void queue_push(queue_t *q,  procent *t) {
  if (q->first == 0) {
    t->next = 0;
    q->first = t;
    q->last = t;
  } else {
    t->next = 0;
    q->last->next = t;
    q->last = t;
  }
}

 void queue_pushsort(queue_t *q,  procent *t) {
   procent *tmp;

  if (q->first == 0) {
    q->first = t;
    q->last = t;
    t->next = 0;
  } else {
    tmp = q->first;
    if (t->param < tmp->param) {
      t->next = tmp;
      q->first = t;
    } else {
      while (tmp->next && (t->param > tmp->next->param))
        tmp = tmp->next;

      if (tmp->next == 0) {
        tmp->next = t;
        t->next = 0;
        q->last = t;
      } else {
        t->next = tmp->next;
        tmp->next = t;
      }
    }
  }
}

  procent* queue_pop(queue_t *q) {
   procent *tmp = q->first;
  if (!tmp)
    return 0;

  q->first = tmp->next;
  if (!q->first)
    q->last = 0;

  return tmp;
}

  procent* queue_peek(queue_t *q) {
  return q->first;
}
uint32 sem_init(sem_t *sem, uint32 value) {
  if (!sem)
    return ERR_PARAMS;

  sem->value = value;
  queue_init(&sem->waitq);
  return ERR_OK;
}

uint32 sem_wait(sem_t *sem) {
  if (!sem)
    return ERR_PARAMS;
  if(active_task==NULL)
    return ERR_PARAMS;

 
  uint32 q=irq_disable();//SYS_ENTRY();
  if (sem->value > 0) {
    sem->value -= 1;
  } else {
    active_task->prstate = PR_WAIT;
    queue_push(&(sem->waitq), active_task);
    irq_restore(q);
    //enable_interrupt(MTIM);//
    //syscall(XINU_YIELD);//
    resched();//

  }
 
  irq_restore(q);
  return ERR_OK;
}



uint32 sem_signaln(sem_t *sem, int n) {
     procent *tmp;

    if (!sem || n == 0) {
        return ERR_PARAMS;  // Devuelve un error si el semáforo es nulo o n es 0
    }
    if (active_task == NULL) {
        return ERR_PARAMS;  // Devuelve un error si no hay una tarea activa
    }

    uint32 q = irq_disable();  // Deshabilita las interrupciones
    while (n > 0 && (tmp = queue_pop(&(sem->waitq)))) {
        // Despierta a los procesos en espera
        tmp->prstate = PR_READY;
        queue_push(&ready_queue, tmp);
        n--;
    }

    // Si aún quedan señales para incrementar y no hay procesos en espera
    sem->value += n;

    irq_restore(q);  // Restaura las interrupciones

    resched();  // Reprograma si es necesario
    return ERR_OK;
}


uint32 sem_signal(sem_t *sem) {
   procent *tmp;

  if (!sem)
    return ERR_PARAMS;
  if(active_task==NULL)
    return ERR_PARAMS;

  uint32 q=irq_disable(); 
  tmp = queue_pop(&(sem->waitq));
  if (tmp) {
 
      tmp->prstate = PR_READY;
      queue_push(&ready_queue, tmp);
 
      irq_restore(q);
     
      resched();//
  } else {
    sem->value += 1;
  }
 
  irq_restore(q);
  //syscall(XINU_YIELD);//
  resched();
  return ERR_OK;
}

uint32 sem_count(sem_t *sem) {
    if (!sem) {
        return ERR_PARAMS;  // Devuelve un error si el puntero al semáforo es nulo
    }

    uint32 q = irq_disable();  // Deshabilita las interrupciones para asegurar la lectura del valor
    uint32 count = sem->value;  // Captura el valor actual del semáforo
    irq_restore(q);  // Restaura las interrupciones

    return count;  // Devuelve el valor del semáforo
}