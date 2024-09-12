/* interrupt.h */
#include <stdint.h>
 
//extern uint64_t clockticks; // rolls over after 2^64/96MHz = 6089.1097 years
extern uint64_t   cycleCount(void);
extern void delay(uint32_t usec);
//extern void delay_ms(uint32_t delay_ms);
extern void delay_us(uint32_t delay_us);
/* in file intr.S */
extern	uint32_t disable(void);
extern	void	enable(void);
extern	void	restore(uint32);
extern void init_timer(uint64_t interval);
extern void clkupdate(uint64_t cycles);
extern void clkhandler(void);

extern void enable_interrupt_status();
extern void enable_interrupt(unsigned int num);
extern void disable_interrupt_status();
extern void disable_interrupt(unsigned int num);
extern void clear_interrupts();
extern void soft_trap();


extern unsigned int irq_enable(void);
extern unsigned int irq_disable(void);
extern void irq_restore(unsigned int state);