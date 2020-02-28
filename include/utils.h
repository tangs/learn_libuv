#include <stdio.h>

void print_buf_hex(const char* buf, size_t len) {
  for (int i = 0; i < ((len + 15) >> 4); ++i) {
    for (int j = 0; j < 16; ++j) {
      int idx = (i << 4) + j;
      char ch = idx < len ? buf[idx] : ' ';
      printf("%02x ", ch);
    }
    printf("\t\t");
    for (int j = 0; j < 16; ++j) {
      int idx = (i << 4) + j;
      char ch = idx < len ? buf[idx] : ' ';
      printf("%c", ch == '\n' ? ' ' : ch);
    }
    printf("\n");
  }
  printf("\n");
}
