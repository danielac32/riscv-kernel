//#include <stdint.h>
//#include <kernel.h>
//#include <interrupt.h>
//#include <string.h>
 
#include <xinu.h>
#include <rv.h>


 


void clkhandler(void){
 
     procent *temp;

    clkupdate(100);
     procent *tmp = queue_peek(&sleep_queue);
    sys_time += 1;
 
    while (tmp) {
        if (sys_time > tmp->param) { // si se cumplio el tiempo sleep
            tmp = queue_pop(&sleep_queue);//sacalo del queue sleep
            ready(tmp->pid);
            tmp = queue_peek(&sleep_queue);
        } else {
            break;
        }
    }
    if(sys_time >= preempt + QUANTUM){
       preempt = sys_time;
       syscall(XINU_YIELD);//
       //resched();
    } 

	 
 

  
}
