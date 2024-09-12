#include <stdint.h>
#include <kernel.h>
#include <interrupt.h>
#include <string.h>
#include <rv.h>
#include <stdio.h>

struct irq_context
{
    unsigned int pc;
    unsigned int status;
    unsigned int cause;
    unsigned int reg[NUM_GP_REG];
};


extern void    exception_dump_ctx(struct irq_context *ctx);
extern struct irq_context * exceptionHandler(struct irq_context *ctx);
extern struct irq_context *do_syscall(struct irq_context *ctx);
