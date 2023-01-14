#include "headers/linker.h"
#include <stdio.h>


int main()
{
    elf_t elf;
    parse_elf("./files/exe/sum.elf.txt", &elf);
    free_elf(&elf);

    return 0;
}