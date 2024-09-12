#include <xinu.h>
#include <rv.h>
#include <syscall.h>
#include <littlefs.h>

syscall_t syscallp;
littlefs_t disk;

static	void sysinit(); 	/* Internal system initialization	*/
 
 
procent	proctab[NPROC];	/* Process table			*/
procent *active_task=NULL;
queue_t ready_queue;
int	prcount;		/* Total number of live processes	*/
 




void ilde(){
    int a;
    while(1){
        kprintf("ilde %d\n",a );
       // syscall(XINU_YIELD);
        //syscall(XINU_YIELD);//resched();
        //printf("ilde1\n");
        sleep(10000);
        a++;
    }
}

void ilde2(){
    int a;
    while(1){
        kprintf("    		ilde2: %d\n",a );
       // syscall(XINU_YIELD);
        //syscall(XINU_YIELD);//resched();
        //printf("        ilde2\n");   
        sleep(1000);
        a++;
    }
}

void nullprocess(void) {
//resume(create(shell, 4096, 3, "shell", 0));
//ready(create(ilde, 2048, 3, "ilde", 0));
//while(1){

//}
}


void	nulluser(){
struct	memblk	*memptr;	/* Ptr to memory block		*/
uint32	free_mem;	
irq_disable();
sysinit();
preempt = QUANTUM;
 
kprintf ("Build date: %s %s\n\n", __DATE__, __TIME__);

heap_init((void *)&_end);
uint32_t f=heap_free();
kprintf("free memory: %d\n",f);

kprintf("%10d bytes of Xinu code.\n",
	(uint32)&_etext - (uint32)&_text);
kprintf("           [0x%08X to 0x%08X]\n",
	(uint32)&_text, (uint32)&_etext - 1);
kprintf("%10d bytes of data.\n",
	(uint32)&_ebss - (uint32)&_sdata);
kprintf("           [0x%08X to 0x%08X]\n\n",
	(uint32)&_sdata, (uint32)&_ebss - 1);



kprintf("upload fs at (%08x)\n",(uint32)&fs);
upload_fs((uint32)&fs);
int ret;
int disk_free;
littlefs_init(&disk, &config, 500000);
#if 0
kprintf("startup_test: format    ");
ret = disk.format(disk.lfs, &config);
if(ret) 
{
    kprintf("[FAIL]\n");
    kprintf("Error Code: %d\n", ret);
    return ;
}
kprintf("[ OK ]\n");
#endif
kprintf("startup_test: mount     ");
ret = disk.mount(disk.lfs, &config);
if(ret) 
{
    kprintf("[FAIL]\n");
    kprintf("Error Code: %d\n", ret);
    return ;
}

kprintf("[ OK ]\n");
disk_free = disk.free();
kprintf("startup_test: available size = %d Bytes\n", disk_free);



//timel = 1;
//timeh = 0;
int pid=create(NULL, 2048, 1, "nullp", 0);
active_task =& proctab[pid];
active_task->prstate=PR_CURR;
ready(create(ilde, 2048, 3, "ilde", 0));
//ready(create(ilde2, 2048, 3, "ilde2", 0));
//ready(create(main, 4096, 3, "main", 0));

//ready(create(nullprocess, 2048, 3, "ilde", 0));
//ready(create(shell, 4096, 3, "shell", 0));
timel = 100;
timeh = 0;
irq_enable();
//timeh = 0;
//init_timer(10);
 
//initos();
//
for(;;);//
}


static	void	sysinit()
{
	int32	i;
		procent	*prptr;		/* Ptr to process table entry	*/
 
    clkinit();
    syscall_init(&syscallp);
	/* Platform Specific Initialization */
    //meminit();
	  //platinit();
	clear_interrupts();
	enable_interrupt_status();
	enable_interrupt(MTIM);
	enable_interrupt(MSIT);
    queue_init(&ready_queue);
	//kprintf(CONSOLE_RESET);
	for (int i = 0; i < 10; ++i)
	{
		kprintf("\n");
	}
	
	prcount = 0;
//	prcount = 1;


	/* Initialize process table entries free */

	for (i = 0; i < NPROC; i++) {
		prptr = &proctab[i];
		prptr->prstate = PR_FREE;
		prptr->prname[0] = NULLCH;
		prptr->prstkbase = NULL;
		prptr->elf = FALSE;
	}



	
	for (i = 0; i < NDEVS; i++) {
		init(i);
	}
	return;
}
