/*
 * General/utility function prototypes
 */
void check_args(int);
void usage();

FILE *open_or_exit(const char *);
void close_file(FILE *);

Elf64_Half make_halfword(unsigned char, unsigned char);

/*
 * ELF header print and parsing prototypes
 */
void parse_elfhdr64(Elf64_Ehdr *, FILE *);
void parse_elfhdr64_ident(unsigned char[], FILE *);
Elf64_Half parse_elfhdr64_type(FILE *);
Elf64_Half parse_elfhdr64_machine(FILE *);

void print_elfhdr64(Elf64_Ehdr *);
void print_elfhdr64_ident(unsigned char[]);
void print_elfhdr64_magicnum(unsigned char[]);
void print_elfhdr64_elfclass(unsigned char);
void print_elfhdr64_encoding(unsigned char);
void print_elfhdr64_version(unsigned char);
void print_elfhdr64_type(Elf64_Half);
void print_elfhdr64_machine(Elf64_Half);
