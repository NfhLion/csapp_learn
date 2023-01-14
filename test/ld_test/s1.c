/*--------------------------------------------------*/
/* function (bind, type, section index)             */
/*--------------------------------------------------*/

extern void f1();       // global, notype, undefined
extern void f2() {}     // global, func, .text

void f3();              // global, notype, undefined
void f4() {}            // global, func, .text

__attribute__((weak)) extern void f5();     // weak, notype, undefined
__attribute__((weak)) extern void f6() {};  // weak, func, .text

__attribute__((weak)) void f7();        // weak, notype, undefined
__attribute__((weak)) void f8() {};     // weak, func, .text

// warning: ‘f9’ used but never defined - fallback to f3
static void f9();       // global, notype, undefined
static void fa() {}     // local, func, .text

/*--------------------------------------------------*/
/* object (bind, type, section index)               */
/*--------------------------------------------------*/

extern int d1;          // global, notype, undefined
// warning: ‘d2’ initialized and declared ‘extern’ - fallback to d8
extern int d2 = 0;      // global, object, .bss
// warning: ‘d3’ initialized and declared ‘extern’ - fallback to d9
extern int d3 = 1;      // global, object, .data

// fallback to d5 - static define
static int d4;          // local, object, .bss
static int d5 = 0;      // local, object, .bss
static int d6 = 1;      // local, object, .data

int d7;         // gloabl, object, COMMON
int d8 = 0;     // gloabl, object, .bss
int d9 = 1;     // global, object, .data

// fallback to db
__attribute__((weak)) int da;       // weak, object, .bss
__attribute__((weak)) int db = 0;   // weak, object, .bss
__attribute__((weak)) int dc = 1;   // weak, object, .data


void reference()
{
    f1(); f2(); f3(); f4(); f5();
    f6(); f7(); f8(); f9(); fa();
    d1 = 2; d2 = 2; d3 = 2; d4 = 2; 
    d5 = 2; d6 = 2; d7 = 2; d8 = 2; 
    d9 = 2; da = 2; db = 2; dc = 2;
}