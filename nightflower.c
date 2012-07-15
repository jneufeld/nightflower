#include <stdio.h>
#include <stdlib.h>

#include "elf_parse.h"
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
 * Utility functions
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
