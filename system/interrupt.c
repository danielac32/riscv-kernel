#include <stdint.h>
#include <kernel.h>
#include <interrupt.h>
#include <string.h>
#include <rv.h>

volatile uint64_t *mtimecmp = (uint64_t *)(CLINT_BASE_ADDR + CLINT_MTIMECMP_OFFSET);
volatile uint64_t *mtime = (uint64_t *)(CLINT_BASE_ADDR + CLINT_MTIME_OFFSET);

uint64_t   cycleCount(void) { 
	    return *mtime;// - (uint32_t)(SysTick->VAL & STK_LOAD_RELOAD); 
}


void delay(uint32_t usec) {
     uint64_t now = get_cyc_count();
      //then = now + usec * clockspeed_hz / (usec/sec)
     uint64_t then = now + 1 * usec;
     while (get_cyc_count() < then)
         asm volatile("add x0,x0,x0");
 }

void init_timer(uint64_t interval){
 	uint64_t current_time = *mtime;
    *mtimecmp = current_time + interval;
} 

void clkupdate(uint64_t cycles)
{
    *mtimecmp = (*mtime) + cycles;
}


 inline __attribute__((always_inline)) unsigned int irq_enable(void)
{
    /* Enable all interrupts */
    unsigned state;

    __asm__ volatile (
        "csrrs %[dest], mstatus, %[mask]"
        :[dest]    "=r" (state)
        :[mask]    "i" (MSTATUS_MIE)
        : "memory"
        );
    return state;
}


 inline __attribute__((always_inline)) unsigned int irq_disable(void)
{

    unsigned int state;

    __asm__ volatile (
        "csrrc %[dest], mstatus, %[mask]"
        :[dest]    "=r" (state)
        :[mask]    "i" (MSTATUS_MIE)
        : "memory"
        );

    return state;
}


 inline __attribute__((always_inline)) void irq_restore(
    unsigned int state)
{
    /* Restore all interrupts to given state */
    __asm__ volatile (
        "csrw mstatus, %[state]"
        : /* no outputs */
        :[state]   "r" (state)
        : "memory"
        );
}
