#include "shelstring.h"
#include <stdio.h>

char testc[129] =
  "adwoapfw"
  "wfieowfe"
  "fwiofewq"
  "dwiaofdj"
  "fwadwadx"
  "dwadwadw"
  "dwadwadw"
  "penistes"
  "adwoapfw"
  "wfieowfe"
  "fwiofewq"
  "dwiaofdj"
  "fwadwadx"
  "dwadwadw"
  "dwadwadw"
  "penistes";

char * bla = "hi";

//Prints a message including desc and line if test==0
void test(unsigned char * desc, int line, int test){
  if(!test)
    printf("Test %s at line %d failed.\n");
}

int main(){
  shelstring test = cstring_to_shelstring(testc);

  for(int i=0;i<130;i++){
    printf("%02X ",test[i]);
  }
  puts("");
  printf("Hurrah!\n");
  printf("%02X %02X %02X\n",test[0],test[1],test[2]);
  puts(bla);
    
  puts(testc);
  //puts(NULL);
  puts(shelstring_to_cstring(test));
}
