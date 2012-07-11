/*
 * General/utility function prototypes
 */
void check_args(int);
void usage();

FILE *open_or_exit(const char *);
void close_file(FILE *);

/*
 * ELF header print and parsing prototypes
 */
void parse_elfhdr64(Elf64_Ehdr *, FILE *);

void print_elfhdr64(Elf64_Ehdr *);
void print_elfhdr64_ident(unsigned char[]);
void print_elfhdr64_magicnum(unsigned char[]);
void print_elfhdr64_elfclass(unsigned char);
void print_elfhdr64_encoding(unsigned char);
void print_elfhdr64_version(unsigned char);
