#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "headers/common.h"
#include "headers/linker.h"

// print test info 
// begin
static void print_sh_entry(sh_entry_t *sh)
{
    printf("%s\t%lx\t%lu\t%lu\n",
        sh->sh_name,
        sh->sh_addr,
        sh->sh_offset,
        sh->sh_size);
}

static void print_symtab_entry(st_entry_t *ste)
{
    printf("%s\t%d\t%d\t%s\t%lu\t%lu\n",
        ste->st_name,
        ste->bind,
        ste->type,
        ste->st_shndx,
        ste->st_value,
        ste->st_size);
}
// end

static int parse_table_entry(char *str, char ***ent)
{
    // column0,column1,column2,column3,...
    // parse line as table entries
    int count_col = 1;
    int len = strlen(str);

    for (int i = 0; i < len; i++)
    {
        if (str[i] == ',')
        {
            count_col ++;
        }
    }

    char **arr = (char **)malloc(count_col * sizeof(char *));
    *ent = arr;

    int col_index = 0;
    int col_width = 0;
    char col_buf[32];
    for (int i = 0; i < len + 1; i++)
    {
        if (str[i] == ',' || str[i] == '\0')
        {
            assert(col_index < count_col);

            char *col = (char *)malloc((col_width + 1) * sizeof(char));
            for (int j = 0; j < col_width; j++)
            {
                col[j] = col_buf[j];
            }
            col[col_width] = '\0';

            // update
            arr[col_index] = col;
            col_index++;
            col_width = 0; 
        }
        else
        {
            assert(col_width < 32);
            col_buf[col_width] = str[i];
            col_width ++;
        }
    }
    return count_col;
}

static void parse_sh(char *str, sh_entry_t *sh)
{
    // .text,0x0,4,22
    char **cols;
    int num_cols = parse_table_entry(str, &cols);
    assert(num_cols == 4);

    assert(sh != NULL); 
    strcpy(sh->sh_name, cols[0]);
    sh->sh_addr = string2uint(cols[1]);
    sh->sh_offset = string2uint(cols[2]);
    sh->sh_size = string2uint(cols[3]);

    for (int i = 0; i < num_cols; ++ i)
    {
        free(cols[i]);
    }
    free(cols);
}

static void parse_symble(char *str, st_entry_t *ste)
{
    // sum,STB_GLOBAL,STT_FUNC,.text,0,22
    char **cols;
    int num_cols = parse_table_entry(str, &cols);
    assert(num_cols == 6);

    assert(ste != NULL);
    strcpy(ste->st_name, cols[0]);

    if (strcmp(cols[1], "STB_LOCAL") == 0)
    {
        ste->bind = STB_LOCAL;
    }
    else if (strcmp(cols[1], "STB_GLOBAL") == 0)
    {
        ste->bind = STB_GLOBAL;
    }
    else if (strcmp(cols[1], "STB_WEAK") == 0)
    {
        ste->bind = STB_WEAK;
    }
    else
    {
        printf("symble bind is Failed!\n");
        exit(2);
    }

    if (strcmp(cols[2], "STT_NOTYPE") == 0)
    {
        ste->type = STT_NOTYPE;
    }
    else if (strcmp(cols[2], "STT_OBJECT") == 0)
    {
        ste->type = STT_OBJECT;
    }
    else if (strcmp(cols[2], "STT_FUNC") == 0)
    {
        ste->type = STB_WEAK;
    }
    else
    {
        printf("symble type is Failed!\n");
        exit(3);
    }

    strcpy(ste->st_shndx, cols[3]);
    ste->st_value = string2uint(cols[4]);
    ste->st_size = string2uint(cols[5]);

    for (int i = 0; i < num_cols; ++ i)
    {
        free(cols[i]);
    }
    free(cols);
}
 
static int read_elf(const char *filename, uint64_t bufaddr)
{
    // open and read file
    FILE *fp = NULL;
    fp = fopen(filename, "r");
    if (fp == NULL)
    {
        printf("unable to open file %s\n", filename);
        exit(1);
    }

    char line[MAX_ELF_FILE_WIDTH];
    int line_counter = 0;

    while (fgets(line, MAX_ELF_FILE_WIDTH, fp) != NULL)
    {
        int len = strlen(line);
        if (len == 0 ||
            (len >= 1 && (line[0] == '\n' || line[0] == '\r')) ||
            (len >= 2 && (line[0] == '/' && line[1] == '/')))
        {
            continue;
        }

        // check if is empty or white line
        int iswhite = 1;
        for (int i = 0; i < len; ++ i)
        {
            iswhite = iswhite && (line[i] == ' ' || line[i] == '\t' || line[i] == '\r');
        }
        if (iswhite == 1)
        {
            continue;
        }

        // to this line, this line is not white and contains information

        if (line_counter < MAX_ELF_FILE_LENGTH)
        {
            // store this line to buffer[line_counter]
            uint64_t addr = bufaddr + line_counter * MAX_ELF_FILE_WIDTH * sizeof(char);
            char *linebuf = (char *)addr;

            int i = 0;
            while (i < len && i < MAX_ELF_FILE_WIDTH)
            {
                if ((line[i] == '\n') || 
                    (line[i] == '\r') || 
                    ((i + 1 < len) && (i + 1 < MAX_ELF_FILE_WIDTH) && line[i] == '/' && line[i + 1] == '/'))
                {
                    break;
                }
                linebuf[i] = line[i];
                i ++;
            }
            linebuf[i] = '\0';
            line_counter ++;
        }
        else
        {
            printf("elf file %s is too long (>%d)\n", filename, MAX_ELF_FILE_LENGTH);
            fclose(fp);
            exit(1);
        }
    }

    fclose(fp);
    assert(string2uint((char *)bufaddr) == line_counter);
    return line_counter; 
}

void parse_elf(char *filename, elf_t *elf)
{
    assert(elf != NULL);

    int line_count = read_elf(filename, (uint64_t)(&(elf->buffer)));
    for (int i = 0; i < line_count; i++) 
    {
        printf("[%d]\t%s\n", i, elf->buffer[i]);
    }
    elf->line_count = line_count;

    // parse section headers
    elf->sht_count = string2uint(elf->buffer[1]);
    elf->sht = malloc(elf->sht_count * sizeof(sh_entry_t));
    memset(elf->sht, 0, elf->sht_count * sizeof(sh_entry_t));

    sh_entry_t *symt_sh = NULL;
    for (int i = 0; i < elf->sht_count; i++)
    {
        parse_sh(elf->buffer[2 + i], &(elf->sht[i]));
        print_sh_entry(&(elf->sht[i]));

        if (strcmp(elf->sht[i].sh_name, ".symtab") == 0)
        {
            symt_sh = &(elf->sht[i]);
        }
    }

    assert(symt_sh != NULL);

    // parse symble table
    elf->symt_count = symt_sh->sh_size;
    elf->symt = malloc(elf->symt_count * sizeof(st_entry_t));
    memset(elf->symt, 0, elf->symt_count * sizeof(st_entry_t));

    for (int i = 0; i < elf->symt_count; i++)
    {
        parse_symble(elf->buffer[symt_sh->sh_offset + i], &(elf->symt[i]));
        print_symtab_entry(&(elf->symt[i]));
    }
}

void free_elf(elf_t *elf)
{
    if (elf->sht != NULL)
    {
        free(elf->sht);
    }
    if (elf->symt != NULL)
    {
        free(elf->symt);
    }
}