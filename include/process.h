/* process.h - isbadpid */

/* Maximum number of processes in the system */
#ifndef PROCESS_H
#define PROCESS_H
#include <stdbool.h>

#ifndef NPROC
#define	NPROC		8
#endif		

 
/* Process state constants */

#define	PR_FREE		0	/* Process table entry is unused	*/
#define	PR_CURR		1	/* Process is currently running		*/
#define	PR_READY	2	/* Process is on ready queue		*/
#define	PR_RECV		3	/* Process waiting for message		*/
#define	PR_SLEEP	4	/* Process is sleeping			*/
#define	PR_SUSP		5	/* Process is suspended			*/
#define	PR_WAIT		6	/* Process is on semaphore queue	*/
#define	PR_RECTIM	7	/* Process is receiving with timeout	*/

/* Miscellaneous process definitions */

#define	PNMLEN		20	/* Length of process "name"		*/
#define	NULLPROC	0	/* ID of the null process		*/

/* Process initialization constants */

#define	INITSTK		4096/2	/* Initial process stack size		*/
#define	INITPRIO	20	/* Initial process priority		*/
#define	INITRET		userret	/* Address to which process returns	*/

/* Inline code to check process ID (assumes interrupts are disabled)	*/

#define	isbadpid(x)	( ((pid32)(x) < 0) || \
			  ((pid32)(x) >= NPROC) || \
			  (proctab[(x)].prstate == PR_FREE))

/* Number of device descriptors a process can have open */

#define NDESC		3	/* must be odd to make procent 4N bytes	*/

/* Definition of the process table (multiple of 32 bits) */

typedef struct procent {		/* Entry in the process table		*/
	uint16	prstate;	/* Process state: PR_CURR, etc.		*/
    bool8	prhasmsg;
    umsg32	prmsg;

	void	*prstkptr;	/* Saved stack pointer			*/
	void	*prstkbase;	/* Base of run time stack		*/
	uint32	prstklen;	/* Stack length in bytes		*/
	char	prname[PNMLEN];	/* Process name				*/
	int pid;
	pid32	prparent;	/* ID of the creating process		*/
	uint32 param;
	int16	prdesc[NDESC];	/* Device descriptors for process	*/
	bool8 	elf;
	void *img;
	uint32 size;
	struct procent *next;
}procent;

/* Marker for the top of a process stack (used to help detect overflow)	*/
#define	STACKMAGIC	0x0A0AAAA9

extern uint32 sys_time;
extern 	procent *active_task;
extern	procent proctab[];
extern	int32	prcount;	/* Currently active processes		*/
extern	pid32	currpid;	/* Currently executing process		*/
extern void ctxsw(void *, void *, uint8);
 #endif // PROCESS_H