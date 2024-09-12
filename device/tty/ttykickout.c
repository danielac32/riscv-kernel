/* ttykickout.c - ttykickout */

#include <xinu.h>


/*------------------------------------------------------------------------
 *  ttykickout  -  "Kick" the hardware for a tty device, causing it to
 *		     generate an output interrupt (interrupts disabled)
 *------------------------------------------------------------------------
 */
void	ttykickout(
	)
{
	/* Force the UART hardware generate an output interrupt */

	irq_disable();
	ttyhandler(1, 'X', 1);		/* 1, 'X' arguments useless  */
	irq_enable();

	return;
}
