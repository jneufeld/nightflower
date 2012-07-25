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
    uint32_t filesize = get_filesize(bin_file);
    uint32_t position = virt_entry_addr - 0x400000;

    uint8_t possible_mov = 0;
    uint8_t byte = 0;

    fseek(bin_file, position, SEEK_SET);

    while(position <= filesize)
    {
        byte = fgetc(bin_file);

        if(byte == REX_W)
        {
            possible_mov = 1;
        }

        else if(possible_mov && byte == MOV_RR64)
        {
            printf("  MOV RR64 @ %X\n", position - 1);
            possible_mov = 0;
        }

        else if(possible_mov && byte == MOV_IMM)
        {
            printf("  MOV IMM @ %X\n", position - 1);
            possible_mov = 0;
        }

        position++;
    }
}

/*
 * Return the size of the file in bytes.
 */
const uint32_t get_filesize(FILE *bin_file)
{
    /* TODO: Don't read to EOF, read to end of .text section. */
    fseek(bin_file, 0, SEEK_END);
    return ftell(bin_file);
}
