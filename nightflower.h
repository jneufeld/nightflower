/*
 * Byte positions within ELF64 header file
 */
#define ELF64_TYPE_POS      (EI_NIDENT + 0)
#define ELF64_MACHINE_POS   (ELF64_TYPE_POS + 2)
#define ELF64_VERSION_POS   (ELF64_MACHINE_POS + 2)
#define ELF64_ENTRY_POS     (ELF64_VERSION_POS + 4)
#define ELF64_PHOFF_POS     (ELF64_ENTRY_POS + 8)
#define ELF64_SHOFF_POS     (ELF64_PHOFF_POS + 8)
#define ELF64_FLAGS_POS     (ELF64_SHOFF_POS + 8)
#define ELF64_EHSIZE_POS    (ELF64_FLAGS_POS + 4)
#define ELF64_PHENTSIZE_POS (ELF64_EHSIZE_POS + 2)
#define ELF64_PHNUM_POS     (ELF64_PHENTSIZE_POS + 2)
#define ELF64_SHENTSIZE_POS (ELF64_PHNUM_POS + 2)
#define ELF64_SHNUM_POS     (ELF64_SHENTSIZE_POS + 2)
#define ELF64_SHSTRNDX_POS  (ELF64_SHNUM_POS + 2)

/*
 * General/utility function prototypes
 */
void check_args(int);
void usage();

FILE *open_or_exit(const char *);
void close_file(FILE *);

Elf64_Half read_elf64half(FILE *, const unsigned int);
Elf64_Word read_elf64word(FILE *, const unsigned int);
Elf64_Addr read_elf64addr(FILE *, const unsigned int);
Elf64_Half make_elf64half(unsigned char, unsigned char);
Elf64_Word make_elf64word(Elf64_Half, Elf64_Half);
Elf64_Addr make_elf64addr(Elf64_Addr, Elf64_Addr);

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
void print_elfhdr64_entry(Elf64_Addr);
void print_elfhdr64_phoff(Elf64_Off);
void print_elfhdr64_shoff(Elf64_Off);
void print_elfhdr64_flags(Elf64_Word);
void print_elfhdr64_ehsize(Elf64_Half);
void print_elfhdr64_phentsize(Elf64_Half);
void print_elfhdr64_phnum(Elf64_Half);
void print_elfhdr64_shentsize(Elf64_Half);
void print_elfhdr64_shnum(Elf64_Half);
void print_elfhdr64_shstrndx(Elf64_Half);
