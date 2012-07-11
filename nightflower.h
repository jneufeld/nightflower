void usage();
int is_elf64(FILE *);
uint64_t entry_addr64(FILE *);
uint64_t file_size(FILE *);
void inject_payload(FILE *, int);
void set_entrypt(FILE *, int);

