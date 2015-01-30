#include "shelstring.h"
#include <stdio.h>

char testc[129] =
  "adwoapfw"
  "wfieowfe"
  "fwiofewq"
  "dwiaofdj"
  "fwadwad "
  "dwadwadw"
  "dwadwadw"
  "penistes"
  "adwoapfw"
  "wfieowfe"
  "fwiofewq"
  "dwiaofdj"
  "fwadwad "
  "dwadwadw"
  "dwadwadw"
  "penistes";

char * bla = "hi";

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
  printf("%s\n",shelstring_to_cstring(test));
}
