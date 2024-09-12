/* xsh_memstat.c - xsh_memstat */

#include <xinu.h>
#include <stdio.h>
#include <string.h>

static	void	printMemUse(void);
static	void	printFreeList(void);

/*------------------------------------------------------------------------
 * xsh_memstat - Print statistics about memory use and dump the free list
 *------------------------------------------------------------------------
 */
shellcmd xsh_memstat(int nargs, char *args[])
{
	 #if 0
	/* For argument '--help', emit help about the 'memstat' command	*/

	if (nargs == 2 && strncmp(args[1], "--help", 7) == 0) {
		printf("use: %s \n\n", args[0]);
		printf("Description:\n");
		printf("\tDisplays the current memory use and prints the\n");
		printf("\tfree list.\n");
		printf("Options:\n");
		printf("\t--help\t\tdisplay this help and exit\n");
		return 0;
	}

	/* Check for valid number of arguments */

	if (nargs > 1) {
		fprintf(stderr, "%s: too many arguments\n", args[0]);
		fprintf(stderr, "Try '%s --help' for more information\n",
				args[0]);
		return 1;
	}



    struct	memblk	*memptr;	
	uint32	free_mem;	


    /*free_mem = 0;
	for (memptr = memlist.mnext; memptr != NULL;
						memptr = memptr->mnext) {
		free_mem += memptr->mlength;
	}*/
	kprintf(" bytes of free memory.  Free list: %10d\n", heap_free());
    kprintf(" bytes of free memory.  Free list2: %10d\n", heap_freecc());

	
	//printMemUse();
	printFreeList();
#endif
	return 0;
}

