#include <stdio.h>
#include <string.h>
#include <byteswap.h>
//#define BYTETOBINARYPATTERN "%d%d%d%d%d%d%d%d"
/*#define BYTETOBINARY(byte)			\
  (byte & 0x80 ? 1 : 0), \
  (byte & 0x40 ? 1 : 0), \
  (byte & 0x20 ? 1 : 0), \
  (byte & 0x10 ? 1 : 0), \
  (byte & 0x08 ? 1 : 0), \
  (byte & 0x04 ? 1 : 0), \
  (byte & 0x02 ? 1 : 0), \
  (byte & 0x01 ? 1 : 0) */

void print_int(int to_print){
  char *t = (unsigned char *)(&to_print);
  for(int i=0;i<4;i++){
    for(int bit=0b10000000;bit>0;bit = bit>>1){
      printf("%d", (t[i] & bit ? 1 : 0));
    }
    printf(" ");
  }
  printf("\n");
}

int main(){
  unsigned int test = 255;
  unsigned int *test_ptr = &test;
  unsigned char* test__cp = (unsigned char *)test_ptr;
  int hi[3] = {1,2,3};
  printf("%d\n",strlen("hi"));
  //printf("%02X%02X%02X%02X\n",test__cp[0],test__cp[1],test__cp[2],test__cp[3]);
  print_int(test);
  print_int(test >> 3);
  print_int(test << 3);

  puts("");
  test = __bswap_32(test);

  print_int(test);
  print_int(test >> 1);
  print_int(test << 1);
  printf("%d\n",(&test)[1]);
  return 0;
}
