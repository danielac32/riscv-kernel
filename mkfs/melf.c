#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "melf.h"



ELF32_shdr *elf_find_section(ELF32_hdr *ehdr, const char *name)
{
    ELF32_shdr *shdr = (ELF32_shdr *)((char *)ehdr + ehdr->e_shoff);
    ELF32_shdr *shdr_str = (ELF32_shdr *)((char *)ehdr + ehdr->e_shoff +
        ehdr->e_shstrndx * ehdr->e_shentsize);

    for (elf32_half i = 0; i < ehdr->e_shnum; i++) {
        char *section = ((char *)ehdr + shdr_str->sh_off) +
            shdr->sh_name;
        if (!strcmp(section, name))
            return shdr;

        shdr = (ELF32_shdr *)((char *)shdr + ehdr->e_shentsize);
    }

    return 0;
}



uint32_t elf_execve(char *elfData, exec_img *res)
{
    // Open the given file
 

    // Read entire file
    //char *elfData = img;
     
  


    // ELF signature check
    if (elfData[0] != '\177' || elfData[1] != 'E' || elfData[2] != 'L' ||
        elfData[3] != 'F') {
        
        free(elfData);
        return -1;
    }

    uint32_t ELF_OFFSET = 0;
    
    // Load program headers
    ELF32_hdr *ehdr = (ELF32_hdr *)elfData;
    Elf32_Phdr *phdr = (Elf32_Phdr *)(elfData + ehdr->e_phoff);

    // Search for the loadable program header
    for (elf32_half i = 0; i < ehdr->e_phnum; i++) {
        if (phdr->p_type == PT_LOAD) {
            // There should only be one PT_LOAD section...
            if (ELF_OFFSET != 0)
                while (1);
            ELF_OFFSET = (uint32_t)malloc(phdr->p_memsz + 8) & ~7;
            res->start = (void*)ELF_OFFSET; 
            res->size = phdr->p_memsz + 8;
            
            // Copy the program data into RAM for execution
            uint8_t *src = (uint8_t *)elfData + phdr->p_offset;
            uint8_t *dst = (uint8_t *)(ELF_OFFSET + phdr->p_vaddr);
            for (uint32_t j = 0; j < phdr->p_filesz; j++)
                dst[j] = src[j];
        }

        phdr = (Elf32_Phdr *)((char *)phdr + ehdr->e_phentsize);
    }

    // Zero the .bss section
    ELF32_shdr *bssSection = elf_find_section(ehdr, ".bss");
    if (bssSection != 0) {
        uint32_t *bssArray = (elf32_addr *)(ELF_OFFSET + bssSection->sh_addr);
        elf32_word bssSize = bssSection->sh_size / sizeof(void *);
        for (elf32_word i = 0; i < bssSize; i++)
            bssArray[i] = 0;
    }

    // Fix GOT entries if they exist
    ELF32_shdr *gotSection = elf_find_section(ehdr, ".got");
    if (gotSection != 0) {
        elf32_addr *gotArray = (elf32_addr *)(ELF_OFFSET + gotSection->sh_addr);
        elf32_word gotSize = gotSection->sh_size / sizeof(void *);
        for (elf32_word i = 0; i < gotSize; i++)
            gotArray[i] += ELF_OFFSET;
    }

    

     
    //free(elfData);
    
    return (ELF_OFFSET + ehdr->e_entry) ;

    //return 0;
}
