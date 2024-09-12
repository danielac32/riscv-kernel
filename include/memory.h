/* memory.h - roundmb, truncmb, freestk */
#include <kernel.h>
#include <stdint.h>
typedef struct {
	uint32_t size;
	void *next;
} __attribute__ ((packed)) alloc_t;

void heap_init(void *buf);
uint32_t heap_free(void);
#define MAXADDR 0x8007d000//0x80000000 + 512000//1000000

/* Added by linker */

extern	unsigned char	_text;			/* Start of text segment	*/
extern	unsigned char	_etext;			/* End of text segment		*/
extern	unsigned char	_sidata;
extern	unsigned char	_sdata;			/* Start of data segment	*/
extern	unsigned char	_edata;			/* End of data segment		*/
extern	unsigned char	_sbss;			/* Start of bss segment		*/
extern	unsigned char	_ebss;			/* End of bss segment		*/
extern	unsigned int	_end;			/* End of program		*/
 

#define ALIGN(to, x)  ((x+(to)-1) & ~((to)-1))