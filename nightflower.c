#include <stdio.h>
#include <stdlib.h>

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

/*
 * Return unsigned 16bit integer.
 */
Elf64_Half make_halfword(unsigned char upper, unsigned char lower)
{
    return (upper << 8) | lower;
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
    print_elfhdr64_type(elf64_header->e_type);
    print_elfhdr64_machine(elf64_header->e_machine);
}

/*
 * Print identification of a 64bit ELF header.
 */
void print_elfhdr64_ident(unsigned char e_ident[])
{
    print_elfhdr64_magicnum(e_ident);
    print_elfhdr64_elfclass(e_ident[EI_CLASS]);
    print_elfhdr64_encoding(e_ident[EI_DATA]);
    print_elfhdr64_version(e_ident[EI_VERSION]);
}

/*
 * Print a 64bit ELF's magic numbers.
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
 * Print a 64bit ELF's class.
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
 * Print a 64bit ELF's data encoding.
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
 * Print a 64bit ELF's version.
 * TODO: Assert version is EV_CURRENT.
 */
void print_elfhdr64_version(unsigned char version)
{
    printf("Version: %d\n", version);
}

/*
 * Print a 64bit ELF's type.
 */
void print_elfhdr64_type(Elf64_Half type)
{
    char *type_str = NULL;

    if(type == ET_NONE)
        type_str = "No file type";
    else if(type == ET_REL)
        type_str = "Relocatable file";
    else if(type == ET_EXEC)
        type_str = "Executable file";
    else if(type == ET_DYN)
        type_str = "Shared object file";
    else if(type == ET_CORE)
        type_str = "Core file";
    else if(type == ET_LOPROC)
        type_str = "Processor specific file (unknown)";
    else if(type == ET_HIPROC)
        type_str = "Processor specific file (unknown)";
    else
        type_str = "Unknown file type";

    printf("Data encoding: %s\n", type_str);
}

/*
 * Print a 64bit ELF's machine.
 * TODO: (Possibly) investigate why elf.h doesn't contain architecture
 *       definitions.
 */
void print_elfhdr64_machine(Elf64_Half machine)
{
    printf("Machine: %d\n", machine);
}

/*
 * TODO: Parse 64bit ELF header data from binary file.
 */
void parse_elfhdr64(Elf64_Ehdr *elf64_header, FILE *bin_file)
{
    parse_elfhdr64_ident(elf64_header->e_ident, bin_file);
    elf64_header->e_type = parse_elfhdr64_type(bin_file);
    elf64_header->e_machine = parse_elfhdr64_machine(bin_file);
}

/*
 * Parse 64bit ELF header's identification from binary file.
 */
void parse_elfhdr64_ident(unsigned char e_ident[], FILE *bin_file)
{
    fseek(bin_file, EI_MAG0, SEEK_SET);

    e_ident[EI_MAG0] = fgetc(bin_file);
    e_ident[EI_MAG1] = fgetc(bin_file);
    e_ident[EI_MAG2] = fgetc(bin_file);
    e_ident[EI_MAG3] = fgetc(bin_file);

    e_ident[EI_CLASS] = fgetc(bin_file);
    e_ident[EI_DATA] = fgetc(bin_file);
    e_ident[EI_VERSION] = fgetc(bin_file);
}

/*
 * Parse 64bit ELF header's type from binary file.
 * TODO: Consider refactoring into single function that takes location to read
 *       from.
 * TODO: Replace type_location with #define TYPE_LOCATION ?.
 */
Elf64_Half parse_elfhdr64_type(FILE *bin_file)
{
    Elf64_Half type = 0;
    unsigned char lower;
    unsigned char upper;
    const int type_location = EI_NIDENT;

    fseek(bin_file, type_location, SEEK_SET);

    lower = fgetc(bin_file);
    upper = fgetc(bin_file);

    type = make_halfword(upper, lower);
    return type;
}

/*
 * Parse 64bit ELF header's machine architecture from binary file.
 * TODO: Consider refactoring into single function that takes location to read
 *       from.
 * TODO: Replace machine_location with #define MACHINE_LOCATION ?.
 */
Elf64_Half parse_elfhdr64_machine(FILE *bin_file)
{
    Elf64_Half machine = 0;
    unsigned char lower;
    unsigned char upper;
    const int machine_location = EI_NIDENT + 2;

    fseek(bin_file, machine_location, SEEK_SET);

    lower = fgetc(bin_file);
    upper = fgetc(bin_file);

    machine = make_halfword(upper, lower);
    return machine;
}

