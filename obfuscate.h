#include <inttypes.h>
#include <stdio.h>
#include <elf.h>

#define MAX_MOV_LEN     (LEN_MOV_RR64 + 1)

#define REX_W           0x48
#define REX_WB          0x49
#define MOV_RR64        0x89
#define MOV_IMM         0xC7

#define LEN_MOV_RR64    2
#define LEN_MOV_IMM     6

/******************************************************************************
 * ELF64 obfuscation helper datatypes
******************************************************************************/ 

struct mov_instruction
{
    uint64_t addr;
    uint8_t  type;
    uint8_t  instruction[MAX_MOV_LEN];
};

/******************************************************************************
 * ELF64 obfuscation function prototypes
******************************************************************************/ 

void elf_obfuscate(Elf64_Ehdr *, FILE *);
void reorder_instructions(uint64_t, FILE *);
void init_movinstr(struct mov_instruction *,
    FILE *,
    const uint64_t,
    const uint8_t);
void print_movinstr(struct mov_instruction *, struct mov_instruction *);
const uint32_t get_filesize(FILE *);
const uint8_t is_mov(uint8_t);
const uint8_t mov_length(uint8_t);
