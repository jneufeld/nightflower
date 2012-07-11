#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include <elf.h>

#include "nightflower.h"

/******************************************************************************
 * main
 ******************************************************************************/

int main(int argc, char *argv[])
{
    check_args(argc);

    FILE *bin_file = open_or_exit(argv[1]);

    Elf64_Ehdr elf_header;
    parse_elfhdr64(&elf_header, bin_file);
    print_elfhdr64(&elf_header);

    close_file(bin_file);

    return 0;
}

/******************************************************************************
 * General/utility functions
 ******************************************************************************/

/*
 * Terminate and print usage if program args are invalid.
 */
void check_args(int argc) 
{
    if(argc != 2) {
        usage();
        exit(-1);
    }
}

/*
 * Print the program's usage.
 */
void usage()
{
    const char *general = "usage: ./nightbird binary_file";
    printf("%s\n", general);
}

/*
 * Return file pointer or terminate and print error.
 */
FILE *open_or_exit(const char *file_name)
{
    const char *open_mode = "r";
    FILE *bin_file = fopen(file_name, open_mode);

    if(!bin_file) {
        printf("Error: Could not open file (%s)\n", file_name);
        exit(-1);
    }

    return bin_file;
}

/*
 * Safely close an open file pointer.
 */
void close_file(FILE *open_file)
{
    close(open_file);
}

/******************************************************************************
 * ELF print and parsing functions
 ******************************************************************************/

/*
 * TODO: Print contents of a 64bit ELF header.
 */
void print_elfhdr64(Elf64_Ehdr *elf64_header)
{
    print_elfhdr64_ident(elf64_header->e_ident);
}

/*
 * TODO: Print contents of a 64bit ELF header.
 */
void print_elfhdr64_ident(unsigned char e_ident[])
{
    print_elfhdr64_magicnum(e_ident);
    print_elfhdr64_elfclass(e_ident[EI_CLASS]);
    print_elfhdr64_encoding(e_ident[EI_DATA]);
    print_elfhdr64_version(e_ident[EI_VERSION]);
}

/*
 * TODO: Print a 64bit ELF's magic numbers.
 */
void print_elfhdr64_magicnum(unsigned char e_ident[])
{
    printf("Magic numbers: %x%c%c%c\n",
        e_ident[EI_MAG0],
        e_ident[EI_MAG1],
        e_ident[EI_MAG2],
        e_ident[EI_MAG3]);
}

/*
 * TODO: Print a 64bit ELF's class.
 */
void print_elfhdr64_elfclass(unsigned char elf_class)
{
    char *elf_class_str = NULL;

    if(elf_class == ELFCLASSNONE)
        elf_class_str = "Invalid class";
    else if(elf_class == ELFCLASS32)
        elf_class_str = "32bit ELF";
    else if(elf_class == ELFCLASS64)
        elf_class_str = "64bit ELF";
    else
        elf_class_str = "Unknown ELF class";

    printf("ELF class: %s\n", elf_class_str);
}

/*
 * TODO: Print a 64bit ELF's data encoding.
 */
void print_elfhdr64_encoding(unsigned char data_encoding)
{
    char *data_encoding_str = NULL;

    if(data_encoding == ELFDATANONE)
        data_encoding_str = "Invalid data encoding";
    else if(data_encoding == ELFDATA2LSB)
        data_encoding_str = "Little endian";
    else if(data_encoding == ELFDATA2MSB)
        data_encoding_str = "Big endian";
    else
        data_encoding_str = "Unknown data encoding";

    printf("Data encoding: %s\n", data_encoding_str);
}

/*
 * TODO: Print a 64bit ELF's version.
 * TODO: Assert version is EV_CURRENT.
 */
void print_elfhdr64_version(unsigned char version)
{
    printf("Version: %d\n", version);
}

/*
 * TODO: Parse 64bit ELF header data from binary file.
 */
void parse_elfhdr64(Elf64_Ehdr *elf64_header, FILE *bin_file)
{

}
