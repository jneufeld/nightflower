#include <inttypes.h>
#include <stdio.h>
#include <elf.h>

#define REX_W       0x48
#define REX_WB      0x49
#define MOV_RR64    0x89
#define MOV_IMM     0xC7

/******************************************************************************
 * ELF64 obfuscation function prototypes
******************************************************************************/ 

void elf_obfuscate(Elf64_Ehdr *, FILE *);
void reorder_instructions(uint64_t, FILE *);
const uint32_t get_filesize(FILE *);
