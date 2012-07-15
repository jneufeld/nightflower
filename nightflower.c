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
    fclose(open_file);
}

/*
 * Return Elf64_Half value from the given position in a binary file.
 */
Elf64_Half read_elf64half(FILE *bin_file, const unsigned int position)
{
    Elf64_Half value = 0;
    unsigned char lower;
    unsigned char upper;

    fseek(bin_file, position, SEEK_SET);

    lower = fgetc(bin_file);
    upper = fgetc(bin_file);

    value = make_elf64half(upper, lower);
    return value;
}

/*
 * Return Elf64_Half from two bytes.
 */
Elf64_Half make_elf64half(unsigned char upper, unsigned char lower)
{
    return (upper << 8) | lower;
}

/*
 * Return Elf64_Word value from the given position in a binary file.
 */
Elf64_Word read_elf64word(FILE *bin_file, const unsigned int position)
{
    Elf64_Word value = 0;
    Elf64_Half lower;
    Elf64_Half upper;

    fseek(bin_file, position, SEEK_SET);

    lower = read_elf64half(bin_file, position);
    upper = read_elf64half(bin_file, position + 2);

    value = make_elf64word(upper, lower);
    return value;
}

/*
 * Return Elf64_Word from two Elf64_Half.
 */
Elf64_Word make_elf64word(Elf64_Half upper, Elf64_Half lower)
{
    return (upper << 16) | lower;
}

/*
 * Return Elf64_Addr value from the given position in a binary file.
 */
Elf64_Addr read_elf64addr(FILE *bin_file, const unsigned int position)
{
    Elf64_Addr value = 0;
    Elf64_Addr lower;
    Elf64_Addr upper;

    fseek(bin_file, position, SEEK_SET);

    lower = read_elf64word(bin_file, position);
    upper = read_elf64word(bin_file, position + 4);

    value = make_elf64addr(upper, lower);
    return value;
}

/*
 * Return Elf64_Addr from two Elf64_Word.
 */
Elf64_Addr make_elf64addr(Elf64_Addr upper, Elf64_Addr lower)
{
    return (upper << 32) | lower;
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
    print_elfhdr64_version(elf64_header->e_version);
    print_elfhdr64_entry(elf64_header->e_entry);
    print_elfhdr64_phoff(elf64_header->e_phoff);
    print_elfhdr64_shoff(elf64_header->e_shoff);
    print_elfhdr64_flags(elf64_header->e_flags);
    print_elfhdr64_ehsize(elf64_header->e_ehsize);
    print_elfhdr64_phentsize(elf64_header->e_phentsize);
    print_elfhdr64_phnum(elf64_header->e_phnum);
    print_elfhdr64_shentsize(elf64_header->e_shentsize);
    print_elfhdr64_shnum(elf64_header->e_shnum);
    print_elfhdr64_shstrndx(elf64_header->e_shstrndx);
}

/*
 * Print identification of a 64bit ELF header.
 */
void print_elfhdr64_ident(unsigned char e_ident[])
{
    print_elfhdr64_magicnum(e_ident);
    print_elfhdr64_elfclass(e_ident[EI_CLASS]);
    print_elfhdr64_encoding(e_ident[EI_DATA]);
    print_elfhdr64_identversion(e_ident[EI_VERSION]);
}

/*
 * Print a 64bit ELF's magic numbers.
 */
void print_elfhdr64_magicnum(unsigned char e_ident[])
{
    printf("Magic numbers: %X%c%c%c\n",
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
void print_elfhdr64_identversion(unsigned char version)
{
    char *version_str = NULL;

    if(version == EV_NONE)
        version_str = "Invalid version";
    else if(version == EV_CURRENT)
        version_str = "Current version";
    else
        version_str = "Unknown file version";

    printf("Version: %s (%d)\n", version_str, version);
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
 * Print a 64bit ELF's version.
 */
void print_elfhdr64_version(Elf64_Word version)
{
    char *version_str = NULL;

    if(version == EV_NONE)
        version_str = "Invalid version";
    else if(version == EV_CURRENT)
        version_str = "Current version";
    else
        version_str = "Unknown file version";

    printf("Version: %s (%d)\n", version_str, version);
}

/*
 * Print a 64bit ELF's entry point.
 */
void print_elfhdr64_entry(Elf64_Addr address)
{
    const long long unsigned int address_u64 = address;
    printf("Entry address: 0x%llX\n", address_u64);
}

/*
 * Print a 64bit ELF's program header offset.
 */
void print_elfhdr64_phoff(Elf64_Off offset)
{
    const long long unsigned int offset_u64 = offset;
    printf("Program header offset: 0x%llX\n", offset_u64);
}

/*
 * Print a 64bit ELF's section header offset.
 */
void print_elfhdr64_shoff(Elf64_Off offset)
{
    const long long unsigned int offset_u64 = offset;
    printf("Section header offset: 0x%llX\n", offset_u64);
}

/*
 * Print a 64bit ELF's processor specific flags.
 */
void print_elfhdr64_flags(Elf64_Word flags)
{
    printf("Processor flags: 0x%X\n", flags);
}

/*
 * Print a 64bit ELF's header size.
 */
void print_elfhdr64_ehsize(Elf64_Half header_size)
{
    printf("ELF header size: %d\n", header_size);
}

/*
 * Print a 64bit ELF's program header entry size.
 */
void print_elfhdr64_phentsize(Elf64_Half phent_size)
{
    printf("Program header entry size: %d\n", phent_size);
}

/*
 * Print a 64bit ELF's number of program header entries.
 */
void print_elfhdr64_phnum(Elf64_Half entries)
{
    printf("Program header entry count: %d\n", entries);
}

/*
 * Print a 64bit ELF's section header entry size.
 */
void print_elfhdr64_shentsize(Elf64_Half shent_size)
{
    printf("Section header entry size: %d\n", shent_size);
}

/*
 * Print a 64bit ELF's number of section header entries.
 */
void print_elfhdr64_shnum(Elf64_Half entries)
{
    printf("Section header entry count: %d\n", entries);
}

/*
 * Print a 64bit ELF's section name string table index.
 */
void print_elfhdr64_shstrndx(Elf64_Half index)
{
    printf("Section name string table index: %d\n", index);
}

/*
 * TODO: Parse 64bit ELF header data from binary file.
 */
void parse_elfhdr64(Elf64_Ehdr *elf64_header, FILE *bin_file)
{
    parse_elfhdr64_ident(elf64_header->e_ident, bin_file);
    elf64_header->e_type = read_elf64half(bin_file, ELF64_TYPE_POS);
    elf64_header->e_machine = read_elf64half(bin_file, ELF64_MACHINE_POS);
    elf64_header->e_version = read_elf64word(bin_file, ELF64_VERSION_POS);
    elf64_header->e_entry = read_elf64addr(bin_file, ELF64_ENTRY_POS);
    elf64_header->e_phoff = read_elf64addr(bin_file, ELF64_PHOFF_POS);
    elf64_header->e_shoff = read_elf64addr(bin_file, ELF64_SHOFF_POS);
    elf64_header->e_flags = read_elf64word(bin_file, ELF64_FLAGS_POS);
    elf64_header->e_ehsize = read_elf64word(bin_file, ELF64_EHSIZE_POS);
    elf64_header->e_phentsize = read_elf64word(bin_file, ELF64_PHENTSIZE_POS);
    elf64_header->e_phnum = read_elf64word(bin_file, ELF64_PHNUM_POS);
    elf64_header->e_shentsize = read_elf64word(bin_file, ELF64_SHENTSIZE_POS);
    elf64_header->e_shnum = read_elf64word(bin_file, ELF64_SHNUM_POS);
    elf64_header->e_shstrndx = read_elf64word(bin_file, ELF64_SHSTRNDX_POS);
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
