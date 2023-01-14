## `csapp`

### 第三章

#### 一. 汇编指令

1. `rip`是和`text段`有关的寄存器。
2. `rsp`是和`stack`有关的寄存器。
3. `rbp ===> rsp`之间的是和当前被调函数有关的内存。

#### 二. 知识点

1. 一些关于机器代码和它的反汇编表示的特性值得注意:

   - ```assembly
     1 0000000000000000 <multstore> :
     	Offset Bytes				Equlvalent assemblu language
     2 		0:  53					push  %rbx
     3 		1:  48 89 d3			mov   %rdx,%rbx
     4 		4:  e8 00 00 00 00		callq 9 <multstore+Ox9>
     5 		9:  48 89 03			mov   %rax,(%rbx)
     6 		c:  5b					pop   %rbx
     7 		d:  c3					retq
     ```

     

   - `x86-64`的指令长度从1到15个字节不等。常用的指令以及操作数较少的指令所需的字节数少,而那些不太常用或操作数较多的指令所需字节数较多。
   - 设计指令格式的方式是,从某个给定位置开始,可以将字节唯一地解码成机器指令。例如,只有指令`pushq %rbx`是以字节值53开头的。
   - 反汇编器只是基于机器代码文件中的字节序列来确定汇编代码。它不需要访间该程序的源代码或汇编代码。
   - 反汇编器使用的指令命名规则与`GCC`生成的汇编代码使用的有些细微的差别。在我们的示例中,它省略了很多指令结尾的`'q'`。这些后缀是大小指示符,在大多数情况中可以省略。相反,反汇编器给`call`和`ret`指令添加了`'q'`,后缀,同样,省略这些后缀也没有问题。

2. 当指令以寄存器作为目标时，对于生成小于8字节结果的指令,寄存器中剩下的字节会怎么样呢？

   - 对此有两条规则:
     - 生成1字节和2字节数字的指令会保持剩下的字节不变;
     - 生成4字节数字的指令会把高位4个字节置为0。
     - 后面这条规则是作为从`IA32`到`x86-64`的扩展的一部分而采用的。

3. 对于`mov`指令，是这样的：源操作数指定的值是一个立即数，存储在寄存器中或者内存中。目的操作数指定一个位置，要么是一个寄存器要么是一个内存地址。`x86-64`加了一条限制：**传送指令的两个操作数不能都是指向内存位置。将一个值从一个内存位置拷贝到另一个内存位置，需要两条指令——第一条指令是将源值加载到寄存器中，第二条是将该寄存器值写入目的位置。**
4. 大多数情况下，`mov`指令只会更新目的操作数指定的那些寄存器字节或内存位置。唯一例外的是：**`movq`指令以寄存器为目的的时候，它会把该寄存器的高4位设置为0**。造成这个例外的原因是因为`x86-64`采用的惯例，也就是上述`2`所描述的。
5. 对于`movabsq I,R`指令，是这样的：这条指令是用来处理64位立即数数据的，常规的`movq`指令只能以表示为32位补码数字的立即数作为源操作数,然后把这个值符号扩展得到64位的值,放到目的位置。`movabsq`指令能够以**任意64位立即数值作为源操作数,并且只能以寄存器作为目的**。
6. ```assembly
   // 4、5的例子
   movabsq $0x0011223344556677, %rax		%rax = 0011223344556677
   movb 	$-1, %al						%rax = 00112233445566FF
   movw 	$-1, %ax						%rax = 001122334455FFFF
   movl 	$-1, %eax						%rax = 00112233FFFFFFFF
   movq 	$-1, %rax						%rax = FFFFFFFFFFFFFFFF
   ```

   

## GDB

1. 查看当前汇编: `disas`
2. 当前汇编下前进几行: `ni + 行数`
3. 进入当前指令：`si`
4. 查看寄存器：`info r`
5. 查看内存: https://blog.csdn.net/xiaozhiwise/article/details/122732433?

### 杂绪
1. 规约
1. 大端（存储）模式，是指数据的低位保存在内存的高地址中，而数据的高位保存在内存的低地址中;
3. 小端（存储）模式，是指数据的低位保存在内存的低地址中，而数据的高位保存在内存的高地址中;
   - 例如：`Intel x86`系列的CPU。



## 第七章

### 一. ELF文件

<img src="/Users/fenghaining/work/csapp_learn/learnNode/assets/image-20221223144207206.png" alt="image-20221223144207206" style="zoom:50%;" />

**`ELF`文件头信息**：

```c
/* The ELF file header.  This appears at the start of every ELF file.  */

#define EI_NIDENT (16)

typedef struct
{
  unsigned char e_ident[EI_NIDENT];     /* Magic number and other info */						***
  Elf64_Half    e_type;                 /* Object file type */
  Elf64_Half    e_machine;              /* Architecture */
  Elf64_Word    e_version;              /* Object file version */
  Elf64_Addr    e_entry;                /* Entry point virtual address */
  Elf64_Off     e_phoff;                /* Program header table file offset */
  Elf64_Off     e_shoff;                /* Section header table file offset */			***
  Elf64_Word    e_flags;                /* Processor-specific flags */
  Elf64_Half    e_ehsize;               /* ELF header size in bytes */							***
  Elf64_Half    e_phentsize;            /* Program header table entry size */
  Elf64_Half    e_phnum;                /* Program header table entry count */
  Elf64_Half    e_shentsize;            /* Section header table entry size */				***
  Elf64_Half    e_shnum;                /* Section header table entry count */			***
  Elf64_Half    e_shstrndx;             /* Section header string table index */
} Elf64_Ehdr;
```

如上其中我们先不关注与`Program`相关的字段。

<img src="/Users/fenghaining/work/csapp_learn/learnNode/assets/image-20221223145303072.png" alt="image-20221223145303072" style="zoom:50%;" />

- 从上图可以得到`ELF`文件大小是：`size = e_shoff + e_shnum * e_shentsize`

**`Section Header table`，节头部表**:

```c
 === > readelf -S elf_test.o
There are 12 section headers, starting at offset 0x308:

Section Headers:
  [Nr] Name              Type             Address           Offset
       Size              EntSize          Flags  Link  Info  Align
  [ 0]                   NULL             0000000000000000  00000000
       0000000000000000  0000000000000000           0     0     0
  [ 1] .text             PROGBITS         0000000000000000  00000040
       0000000000000016  0000000000000000  AX       0     0     1
  [ 2] .data             PROGBITS         0000000000000000  00000058
       0000000000000010  0000000000000000  WA       0     0     8
  [ 3] .bss              NOBITS           0000000000000000  00000068
       0000000000000000  0000000000000000  WA       0     0     1
  [ 4] .comment          PROGBITS         0000000000000000  00000068
       000000000000002c  0000000000000001  MS       0     0     1
  [ 5] .note.GNU-stack   PROGBITS         0000000000000000  00000094
       0000000000000000  0000000000000000           0     0     1
  [ 6] .note.gnu.propert NOTE             0000000000000000  00000098
       0000000000000020  0000000000000000   A       0     0     8
  [ 7] .eh_frame         PROGBITS         0000000000000000  000000b8
       0000000000000058  0000000000000000   A       0     0     8
  [ 8] .rela.eh_frame    RELA             0000000000000000  00000270
       0000000000000030  0000000000000018   I       9     7     8
  [ 9] .symtab           SYMTAB           0000000000000000  00000110
       0000000000000138  0000000000000018          10     9     8
  [10] .strtab           STRTAB           0000000000000000  00000248
       0000000000000024  0000000000000000           0     0     1
  [11] .shstrtab         STRTAB           0000000000000000  000002a0
       0000000000000067  0000000000000000           0     0     1
Key to Flags:
  W (write), A (alloc), X (execute), M (merge), S (strings), I (info),
  L (link order), O (extra OS processing required), G (group), T (TLS),
  C (compressed), x (unknown), o (OS specific), E (exclude),
  l (large), p (processor specific)
    
对应Section Header的struct为：
typedef struct
{
  Elf64_Word	sh_name;		/* Section name (string tbl index) */			***
  Elf64_Word	sh_type;		/* Section type */												***
  Elf64_Xword	sh_flags;		/* Section flags */												***
  Elf64_Addr	sh_addr;		/* Section virtual addr at execution */		***
  Elf64_Off	sh_offset;		/* Section file offset */									***
  Elf64_Xword	sh_size;		/* Section size in bytes */								***
  Elf64_Word	sh_link;		/* Link to another section */
  Elf64_Word	sh_info;		/* Additional section information */
  Elf64_Xword	sh_addralign;		/* Section alignment */
  Elf64_Xword	sh_entsize;		/* Entry size if section holds table */	***
} Elf64_Shdr; 
```

- `.strtab`不是一个字符串表，而是一个由多个字符串组织成的一个超大的字符串（每单个字符串之间是由\0隔开）。 

**`symbol table`**

- 作用

![image-20230104145904893](/Users/fenghaining/work/csapp_learn/learnNode/assets/image-20230104145904893.png)

- ```c
  typedef struct
  {
    Elf64_Word	st_name;			/* Symbol name (string tbl index) */		表示在str tab里的偏移量
    unsigned char	st_info;		/* Symbol type and binding */						Bind(STB) | type(STT)
    unsigned char st_other;		/* Symbol visibility */
    Elf64_Section	st_shndx;		/* Section index */											表示是哪一个section和UDEF、COMMON等 
    Elf64_Addr	st_value;			/* Symbol value */											表示当前symbol的起始位置
    Elf64_Xword	st_size;			/* Symbol size */												表示该符号的大小
  } Elf64_Sym;
  
  // 符号的位置：
  /*
  Start:
  Elf64_Shdr[Elf64_Sym.st_shndx].sh_offset + Elf64_Sym.st_value
  End:
  Start + Elf64_Sym.size - 1
  
  ELF[Start, End] ---> Symbol
  */ 
  ```

- 映射过程

  ![image-20230104160505686](/Users/fenghaining/work/csapp_learn/learnNode/assets/image-20230104160505686.png)