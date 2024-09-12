/* create.c - create, newpid */

#include <xinu.h>

local	int newpid();

#define	roundew(x)	( (x+3)& ~0x3)

/*------------------------------------------------------------------------
 *  create  -  create a process to start running a procedure
 *------------------------------------------------------------------------
 */



extern void *setupStack(void *stackaddr, void *procaddr,
                 void *retaddr, uint nargs, va_list ap);


pid32	create(
	  void		*procaddr,	/* procedure address		*/
	  uint32	ssize,		/* stack size in bytes		*/
	  pri16		priority,	/* process priority > 0		*/
	  char		*name,		/* name (for debugging)		*/
	  uint32	nargs,		/* number of args that follow	*/
	  ...
	)
{
	intmask 	mask;    	/* interrupt mask		*/
	pid32		pid;		/* stores new process id	*/
		procent	*prptr;		/* pointer to proc. table entry */
	int32		i;
	uint32		*a;		/* points to list of args	*/
	//uint32		*saddr;		/* stack address		*/
    va_list ap;  

	mask = irq_disable();
	 
 
    uint32 *saddr = malloc(ssize);
    memset(saddr, 0, ssize); 

	prcount++;
	pid=newpid();
	if (SYSERR == (int)pid)
    {
        kprintf("error thrnew %s\n",name);
        free(saddr);
        irq_restore(mask);
        return SYSERR;
    }

	prptr = &proctab[pid];
    prptr->elf=FALSE;
    prptr->pid=pid;
    prptr->prhasmsg = FALSE;
    prptr->prparent = (pid32)getpid();
	/* initialize process table entry for new process */
	prptr->prstate = PR_SUSP;	/* initial state is suspended	*/
	prptr->prstkbase = saddr;
	prptr->prstklen = ssize;
	prptr->prname[PNMLEN-1] = NULLCH;
	for (i=0 ; i<PNMLEN-1 && (prptr->prname[i]=name[i])!=NULLCH; i++)
		;
	 
	/* set up initial device descriptors for the shell		*/
	prptr->prdesc[0] = CONSOLE;	/* stdin  is CONSOLE device	*/
	prptr->prdesc[1] = CONSOLE;	/* stdout is CONSOLE device	*/
	prptr->prdesc[2] = CONSOLE;	/* stderr is CONSOLE device	*/
	//va_start(ap, nargs);
    //prptr->prstkptr = setupStack(saddr, procaddr, INITRET, nargs, ap);
    //va_end(ap);
    saddr += ssize;
    va_start(ap, nargs);
    prptr->prstkptr = setupStack(saddr, procaddr, INITRET, nargs, ap);
    va_end(ap);
  
	irq_restore(mask);
	return pid;
}

/*------------------------------------------------------------------------
 *  newpid  -  Obtain a new (free) process ID
 *------------------------------------------------------------------------
 */
local	pid32	newpid(void)
{
	uint32	i;			/* iterate through all processes*/
//	static	pid32 nextpid = 1;	/* position in table to try or	*/
	static	pid32 nextpid = 0;	/* position in table to try or	*/
					/*  one beyond end of table	*/

	/* check all NPROC slots */

	for (i = 0; i < NPROC; i++) {
		nextpid %= NPROC;	/* wrap around to beginning */
		if (proctab[nextpid].prstate == PR_FREE) {
			return nextpid++;
		} else {
			nextpid++;
		}
	}
	return (pid32) SYSERR;
}



/*------------------------------------------------------------------------
 *  getpid  -  Return the ID of the currently executing process
 *------------------------------------------------------------------------
 */
pid32	getpid(void)
{
	return active_task->pid;
}
 
/*------------------------------------------------------------------------
 *  kill  -  Kill a process and remove it from the system
 *------------------------------------------------------------------------
 */
syscall	kill(
	  pid32		pid		/* ID of process to kill	*/
	)
{
 uint32 mask=irq_disable();
  char *state[]={"free ", "curr ", "ready", "recv ", "sleep", "susp ","wait "};

   procent *prptr;
  prptr =& proctab[pid];
  
  free(prptr->prstkbase);

  if(prptr->elf == TRUE){
     //kprintf("killed %s\n", prptr->name);
     free(prptr->img);//free(prptr->img);
     prptr->elf = FALSE;
  }


  switch(prptr->prstate){
        case PR_CURR:
             prptr->prstate=PR_FREE;
 
        break;
        case PR_SLEEP:
             {

              //kprintf("\n");
               procent *tmp = queue_peek(&sleep_queue);
              while (tmp->prstate==PR_SLEEP) {
                    tmp = queue_pop(&sleep_queue);

                    if (tmp->pid==prptr->pid) {
                        //tmp = queue_pop(&sleep_queue);
                        tmp->prstate = PR_FREE;
                 
                        break;
                    }else{
                        queue_push(&sleep_queue, tmp);
                    }
              }
              
             }
        break;
        case PR_READY:
              {

              //kprintf("\n");
               procent *tmp = queue_peek(&ready_queue);
              while (tmp->prstate==PR_READY) {
                    tmp = queue_pop(&ready_queue);

                    if (tmp->pid==prptr->pid) {
                        //tmp = queue_pop(&sleep_queue);
                        tmp->prstate = PR_FREE;
 
                        break;
                    }else{
                        queue_push(&ready_queue, tmp);
                    }
              }
              
             }
     
        break;
        default:
            
            kprintf("default .killed  %d--%s %s\n", prptr->pid,prptr->prname,state[prptr->prstate]);
            prptr->prstate = PR_FREE;
            break;
            
  }
  irq_restore(mask);
  yield();
  
	#if 0
	intmask	mask;			/* Saved interrupt mask		*/
	struct	procent *prptr;		/* Ptr to process's table entry	*/
	int32	i;			/* Index into descriptors	*/

	mask = disable();
	if (isbadpid(pid) || (pid == NULLPROC)
	    || ((prptr = &proctab[pid])->prstate) == PR_FREE) {
		restore(mask);
		return SYSERR;
	}

	if (--prcount <= 1) {		/* Last user process completes	*/
		xdone();
	}

	send(prptr->prparent, pid);
	for (i=0; i<3; i++) {
		//close(prptr->prdesc[i]);
	}
	freestk(prptr->prstkbase, prptr->prstklen);
	//free(prptr->prstkbase);
    if(prptr->elf == TRUE){
    	//kprintf("clean space %d\n",prptr->prstklen);
        free(prptr->img);
        //freemem(prptr->img,prptr->size);
    }
	/*if(riscv1[pid].running){// si hay un vm activa
	   riscv1[pid].running=0;
       #if use_ram == 1
            free(riscv1[pid].memory);
       #endif
       riscv1[pid].prstate = PR_FREE;
       f_close(riscv1[pid].file);
	}*/

	switch (prptr->prstate) {
	case PR_CURR:
		prptr->prstate = PR_FREE;	/* Suicide */
		resched();

	case PR_SLEEP:
	case PR_RECTIM:
		unsleep(pid);
		prptr->prstate = PR_FREE;
		break;

	case PR_WAIT:
		semtab[prptr->prsem].scount++;
		/* Fall through */

	case PR_READY:
		getitem(pid);		/* Remove from queue */
		/* Fall through */

	default:
		prptr->prstate = PR_FREE;
	}

	restore(mask);
	#endif
	return OK;
}


/*------------------------------------------------------------------------
 *  yield  -  Voluntarily relinquish the CPU (end a timeslice)
 *------------------------------------------------------------------------
 */
syscall	yield(void)
{

	resched();

	return OK;
}
  
/*------------------------------------------------------------------------
 *  userret  -  Called when a process returns from the top-level function
 *------------------------------------------------------------------------
 */
void	userret(void)
{
	kill(getpid());			/* Force process to exit */
}

 
 
/*------------------------------------------------------------------------
 *  sleep  -  Delay the calling process n seconds
 *------------------------------------------------------------------------
 */
syscall	sleep(
	  int32	delay		/* Time to delay in seconds	*/
	)
{
	uint32 mask=irq_disable();
  if (delay) {
      active_task->prstate = PR_SLEEP;
      active_task->param = delay + sys_time;//*1000;
      queue_pushsort(&sleep_queue, active_task);
      //queue_push(&sleep_queue, active_task);
  }
  irq_restore(mask);
  //syscall(XINU_YIELD);//
  resched();
	return OK;
}
 

void  resched(void){

#if 1 

  procent *throld;     
  procent *thrnew; 

// throld = active_task;
 throld = &proctab[getpid()];
 if (throld->prstate == PR_CURR) { 
     throld->prstate = PR_READY;
     queue_push(&ready_queue, throld);
 }
 active_task = queue_pop(&ready_queue);
 thrnew = active_task;
 active_task->prstate=PR_CURR;
 preempt = QUANTUM;
 ctxsw(&throld->prstkptr, &thrnew->prstkptr,0);
 

#else

    struct procent *throld;     
    struct procent *thrnew; 

    throld = &proctab[getpid()];

    if (PR_CURR == throld->prstate){
        throld->prstate = PR_READY;
        queue_push(&ready_queue, throld);
    }
    active_task = queue_pop(&ready_queue);
 
    thrnew =& proctab[getpid()];
    thrnew->prstate=PR_CURR;
    ctxsw(&throld->prstkptr, &thrnew->prstkptr,0);
#endif

   #if 0
    struct procent *ptold;	/* Ptr to table entry for old process	*/
	struct procent *ptnew;	/* Ptr to table entry for new process	*/
    intmask mask;
    uint8 asid;
	 
   
    ptold = &proctab[currpid];
    mask = disable();
    if (ptold->prstate == PR_CURR) {  /* Process remains eligible */
		if (ptold->prprio > firstkey(readylist)) {
			restore(mask);
			return;
		}
		ptold->prstate = PR_READY;
		insert(currpid, readylist, ptold->prprio);
	}

	currpid = dequeue(readylist);
	ptnew = &proctab[currpid];
	ptnew->prstate = PR_CURR;
	preempt = QUANTUM;		/* Reset time slice for process	*/
    asid = currpid & 0xff;
  //  ctxsw(&ptold->prstkptr, &ptnew->prstkptr, asid);
    /* old thread returns here when resumed */
 #endif
}

/*------------------------------------------------------------------------
 *  resume  -  Unsuspend a process, making it ready
 *------------------------------------------------------------------------
 */
pri16	resume(
	  pid32		pid		/* ID of process to unsuspend	*/
	)
{

	intmask	mask;			/* Saved interrupt mask		*/
		procent *prptr;		/* Ptr to process's table entry	*/
	pri16	prio;			/* Priority to return		*/

	mask = irq_disable();
	if (isbadpid(pid)) {
		irq_restore(mask);
		return (pri16)SYSERR;
	}
	prptr = &proctab[pid];
	if (prptr->prstate != PR_SUSP) {
		irq_restore(mask);
		return (pri16)SYSERR;
	}
	//prio = prptr->prprio;		/* Record priority to return	*/
	ready(pid);
	irq_restore(mask);
	yield();
	//
	return prio;
}


 status	ready(
	  pid32		pid		/* ID of process to make ready	*/
	)
{
	register  procent *prptr;

	if (isbadpid(pid)) {
		return SYSERR;
	}

	/* Set process state to indicate ready and add to ready list */

	prptr = &proctab[pid];
	prptr->prstate = PR_READY;
	queue_push(&ready_queue, prptr); 
	//insert(pid, readylist, prptr->prprio);
	

	return OK;
}

/*------------------------------------------------------------------------
 *  exit  -  Cause the calling process to exit
 *------------------------------------------------------------------------
 */
void	exit(void)
{
	kill(getpid());		/* Kill the current process */
}
