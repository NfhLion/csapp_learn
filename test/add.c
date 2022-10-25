#include <stdint.h>

uint64_t add(uint64_t a, uint64_t b) {
  uint64_t c = a + b;
  return c;
}

int main() {
  uint64_t a = 0x12340000;
  uint64_t b = 0x0000abcd;
  uint64_t c = add(a, b);

  return 0;
}

/*

0x7fffffffdc10: 0x0000a000
0x7fffffffdc0f: 0x00a00012
0x7fffffffdc0e: 0xa0001234
0x7fffffffdc0d: 0x00123456
0x7fffffffdc0c: 0x12345678
0x7fffffffdc0b: 0x34567887
0x7fffffffdc0a: 0x56788765
0x7fffffffdc09: 0x78876543
0x7fffffffdc08: 0x87654321

*/