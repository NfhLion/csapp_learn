#include <stdio.h>

unsigned int low_bit(unsigned int x) {
    unsigned int a = x & ((~x) + 1);
    return a;
}

int main (int argc, char *argv[]) {

     

    return 0;
}