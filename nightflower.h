/*
 * Byte positions within ELF64 header file
 */
#define ELF64_TYPE_POS      (EI_NIDENT + 0)
#define ELF64_MACHINE_POS   (ELF64_TYPE_POS + 2)
#define ELF64_VERSION_POS   (ELF64_MACHINE_POS + 2)
#define ELF64_ENTRY_POS     (ELF64_VERSION_POS + 4)

/*
 * General/utility function prototypes
 */
void check_args(int);
void usage();

FILE *open_or_exit(const char *);
void close_file(FILE *);

Elf64_Half read_elf64half(FILE *, const unsigned int);
Elf64_Half make_elf64half(unsigned char, unsigned char);
Elf64_Word read_elf64word(FILE *, const unsigned int);
Elf64_Word make_elf64word(Elf64_Half, Elf64_Half);

/*
 * ELF header print and parsing prototypes
 */
void parse_elfhdr64(Elf64_Ehdr *, FILE *);
void parse_elfhdr64_ident(unsigned char[], FILE *);

void print_elfhdr64(Elf64_Ehdr *);
void print_elfhdr64_ident(unsigned char[]);
void print_elfhdr64_magicnum(unsigned char[]);
void print_elfhdr64_elfclass(unsigned char);
void print_elfhdr64_encoding(unsigned char);
void print_elfhdr64_identversion(unsigned char);
void print_elfhdr64_type(Elf64_Half);
void print_elfhdr64_machine(Elf64_Half);
void print_elfhdr64_version(Elf64_Word);
