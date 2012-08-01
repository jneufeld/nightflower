#include "obfuscate.h"

/******************************************************************************
 * ELF64 obfuscation functions
******************************************************************************/

/*
 * Perform obfuscations on a 64bit ELF file.
 */
void elf_obfuscate(Elf64_Ehdr *elf_header, FILE *bin_file)
{
    uint64_t entry_addr = elf_header->e_entry;

    reorder_instructions(entry_addr, bin_file);
}

/*
 * Scan for instructions that can safely be reordered and move them.
 *
 * A significant problem with looking through instructions from an entry
 * address is that we must understand where one instruction ends and another
 * begins, simply searching for a desired (REX prefix and) opcode may result
 * in the location the constant data of some other instruction! The solution
 * seems to be maintaining the lengths of every x86_64 instruction and
 * incrementing the program's instruction pointer by the correct amount to
 * ensure we're always pointing to the beginning of the next instruction.
 * This is, of course, a severely over engineered solution for such a simple
 * project. We'll just cross our fingers and hope for the best.
 */
void reorder_instructions(uint64_t virt_entry_addr, FILE *bin_file)
{
    struct mov_instruction mov1;
    struct mov_instruction mov2;

    uint8_t possible_mov = 0; /* Set if current instruction may be a MOV */
    uint8_t consec_movs  = 0; /* Count of consecutive MOV instructions */
    uint8_t cur_byte     = 0;

    uint32_t filesize = get_filesize(bin_file);
    uint32_t position = virt_entry_addr - 0x400000;

    fseek(bin_file, position, SEEK_SET);

    while(position <= filesize)
    {
        cur_byte = fgetc(bin_file);

        if(cur_byte == REX_W || cur_byte == REX_WB)
        {
            possible_mov = 1;
            position++;
        }

        else if(possible_mov && is_mov(cur_byte))
        {
            struct mov_instruction *cur_mov = (consec_movs) ? &mov2 : &mov1;
            init_movinstr(cur_mov, bin_file, position - 1, cur_byte);

            position += mov_length(cur_byte);

            possible_mov = 0;
            consec_movs++;
        }

        else
        {
            possible_mov = 0;
            consec_movs  = 0;
            position++;
        }

        if(consec_movs >= 2)
        {
            printf("TWO CONSECUTIVE MOVS FOUND: SWAPPING\n");
            print_movinstr(&mov1, &mov2);
            consec_movs = 0;
        }
    }
}

/*
 * Return the size of the file in bytes.
 * TODO: Don't read to EOF, read to end of .text section.
 */
const uint32_t get_filesize(FILE *bin_file)
{
    uint64_t original_position;
    uint64_t filesize;

    original_position = ftell(bin_file);
    
    fseek(bin_file, 0, SEEK_END);
    filesize = ftell(bin_file);
    fseek(bin_file, original_position, SEEK_SET);

    return filesize;
}

/*
 * Initialize a mov_instruction struct. This leaves the file stream pointer
 * pointing to the first byte of the next instruction.
 */
void init_movinstr(struct mov_instruction *mov,
    FILE *bin_file,
    const uint64_t address,
    const uint8_t type)
{
    mov->type = type;
    mov->addr = address;

    fseek(bin_file, -2, SEEK_CUR);

    int length = mov_length(type);
    int i;
    for(i = 0; i <= length; i++)
    {
        mov->instruction[i] = fgetc(bin_file);
    }
}

/*
 * Print contents of two MOV instructions. This is for DEBUGGING and is ugly.
 */
void print_movinstr(struct mov_instruction *mov1, struct mov_instruction *mov2)
{
    printf("MOV1:\n");
    printf("  Address     = 0x%lX\n", mov1->addr);
    printf("  Type        = 0x%X\n", mov1->type);
    printf("  Instruction = ");

    int length1 = mov_length(mov1->type);
    int i;
    for(i = 0; i <= length1; i++)
    {
        printf("0x%X ", mov1->instruction[i]);
    }

    printf("\n");

    printf("MOV2:\n");
    printf("  Address     = 0x%lX\n", mov2->addr);
    printf("  Type        = 0x%X\n", mov2->type);
    printf("  Instruction = ");

    int length2 = mov_length(mov2->type);
    int j;
    for(j = 0; j <= length2; j++)
    {
        printf("0x%X ", mov2->instruction[j]);
    }

    printf("\n\n");
}

/*
 * Return the given opcode if the type is a MOV instruction, else 0.
 */
const uint8_t is_mov(uint8_t mov_opcode)
{
    uint8_t type = 0;

    if(mov_opcode == MOV_RR64 || mov_opcode == MOV_IMM)
    {
        type = mov_opcode;
    }

    return type;
}

/*
 * Return the length of the given MOV type or 0 if not a MOV instruction.
 */
const uint8_t mov_length(uint8_t type)
{
    uint8_t length = 0;

    if(type == MOV_RR64)
    {
        length = LEN_MOV_RR64;
    }

    else if(type == MOV_IMM)
    {
        length = LEN_MOV_IMM;
    }

    return length;
}
