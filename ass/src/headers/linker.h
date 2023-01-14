// include guards to prevent double declaration of any identifiers 
// such as types, enums and static variables
#ifndef LINKER_GUARD
#define LINKER_GUARD

#include <stdint.h>
#include <stdlib.h>

#define MAX_CHAR_SECTION_NAME (32)

// Section header table
typedef struct
{
    char sh_name[MAX_CHAR_SECTION_NAME];
    uint64_t sh_addr;
    uint64_t sh_offset;     // line offset or effective line index
    uint64_t sh_size;
} sh_entry_t;

#define MAX_CHAR_SYMBOL_NAME (64)

// Bind type
typedef enum
{
    STB_LOCAL,
    STB_GLOBAL,
    STB_WEAK
} st_bind_t;

// Type type
typedef enum
{
    STT_NOTYPE,
    STT_OBJECT,
    STT_FUNC
} st_type_t;

// Symble
typedef struct
{
    char st_name[MAX_CHAR_SYMBOL_NAME];
    st_bind_t bind;
    st_type_t type;
    char st_shndx[MAX_CHAR_SECTION_NAME];
    uint64_t st_value;      // in-section offset
    uint64_t st_size;       // count of lines of symbol
} st_entry_t;

#define MAX_ELF_FILE_LENGTH (64)    // max 64 effective lines
#define MAX_ELF_FILE_WIDTH (128)    // max 128 chars per line
// ELF file struct
typedef struct
{
    char buffer[MAX_ELF_FILE_LENGTH][MAX_ELF_FILE_WIDTH];
    uint64_t line_count;                                        // 

    uint64_t sht_count;
    sh_entry_t *sht; 

    uint64_t symt_count;
    st_entry_t *symt;
} elf_t;

void parse_elf(char *filename, elf_t *elf);
void free_elf(elf_t *elf);

#endif // LINKER_GUARD