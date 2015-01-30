/* Format: length-prefixed string
 * length is a series of 7-bit peices, with the LSBit being an indicator bit, indicating whether the next byte is part of the length
 * EG: 0000 0100|0100 1000 0100 1001 is the string "HI" in this format
 *     -length--|------string-------
 *
 * EG: 0000 0001 0000 0000|01010011 01101000 01100101 01101100.....01110110 01100001 01100011 01110101 
 *     ------length-------|--------------  string of length 128  -------------------------------------
 *
 * The length is LSB first - endianness determined by coin flip
 * This is a library to implement said string format, with equivelent functions to that of C strings
 */

#define DEBUG 1

#include <string.h>
#include <stdlib.h>
#include "shelstring.h"
#ifdef DEBUG
# include <stdio.h>
#endif

#define MAX_SIZES_LENGTH 4

//2^(7*n)
const long max_sizes[MAX_SIZES_LENGTH] = {127,16384,2097152,268435456};

short shelint_length_from_cint(unsigned int cint){
  short length_length = 1;
  while(max_sizes[length_length-1] <= cint){
#ifdef DEBUG
    puts("In the while loop");
#endif
    length_length++;
    if(length_length == (MAX_SIZES_LENGTH+1)){
      return -1;
    }
  }
#ifdef DEBUG
  printf("shelint_length_from_cint(%d) = %d\n", cint, length_length);
#endif
  return length_length;
}

void cint_to_shelint(unsigned int cint, int length_length, shelstring ptr){
#ifdef LITTLE_ENDIAN
  //puts("all is well!");
  for(int i=0;i<length_length;i++){
    cint = cint << 1;
    ptr[i] = ((&cint)[i] | 0x01);
# ifdef DEBUG
    printf("in for loop: %02X %02X\n",(&cint)[i],ptr[i]);
# endif
  }
# ifdef DEBUG
  printf("cint_to_shelint(%d,%d,%02X) = %02X %02X\n", cint, length_length, ptr, ptr[0], ptr[1]);
# endif
#else
#error "Big endian not implemented... yet"
#endif
}


shelstring chars_to_shelstring(unsigned char * cstring,size_t size){
  static shelstring shelstring;
  //size_t cstring_len = strlen(cstring);
  size_t cstring_len = size;
  short length_length = shelint_length_from_cint(cstring_len);
  
  if(!(shelstring = malloc(length_length + cstring_len))){
    return 0;
  }
  cint_to_shelint(cstring_len, length_length, shelstring);
  unsigned char * shelstring_chars = shelstring+length_length;
  for(int i=0;i<cstring_len;i++){
    shelstring_chars[i] = cstring[i];
  }
  return shelstring;
}

shelstring cstring_to_shelstring(unsigned char * cstring){
  return chars_to_shelstring(cstring,strlen(cstring));
}

unsigned int shelstrlen(shelstring shelstring){ //AKA shelint to cint
  unsigned int cint = 0;
#ifdef DEBUG
  printf("shelstrlen ");
#endif
  if(!(shelstring[0] & 0x01)){
#ifdef LITTLE_ENDIAN
    (&cint)[0] = shelstring[0] >> 1;
#elif BIG_ENDIAN
    (&cint)[sizeof(cint)] = shelstring[0] >> 1;
#endif
  }else{ //TODO: Fix this algoritm for lengths of lengths > 7 (or maybe 8 works?)
    int i = -1;
    do{
      i++;
#ifdef LITTLE_ENDIAN
      (&cint)[i] =
#elif BIG_ENDIAN // No idea if big endian works, beware
	(&cint)[sizeof(cint) - i] = //TODO: test if this works
#endif
	(shelstring[i] >> i+1) | ((shelstring[i] & 0x01) ?  shelstring[i+1] << 7-i : 0x00);
#ifdef DEBUG
      printf("%02X ",shelstring[i]);
#endif
    }while(shelstring[i] & 0x01);
  }
#ifdef DEBUG
  printf(" = %d\n", cint);
#endif
  return cint;
}

unsigned char * shelstring_to_chars(shelstring shelstring,
				    unsigned int *length_out){
  return shelstring_to_chars_with_addon(shelstring,length_out,0);
}
  
unsigned char * shelstring_to_chars_with_addon(shelstring shelstring,
					       unsigned int *length_out,
					       unsigned int addon_length){
  *length_out = shelstrlen(shelstring);
  unsigned char * chars = malloc(*length_out + addon_length);
  if(!chars) //if malloc failed
    return chars; //then return null
  while(shelstring[0] & 0x01)
    shelstring++;
  //shelstring now points to the last byte of the length, so increment one more to get to the juicy characters
  for(int i=0;i<*length_out;i++)
    chars[i] = shelstring[i];
  return chars;
}

unsigned char * shelstring_to_cstring(shelstring shelstring){
  unsigned int length;
  unsigned char * cstring =
    shelstring_to_chars_with_addon(shelstring,&length,1);
  if(!cstring)
    return NULL;
  cstring[length] = 0x00; //set the last char to a null.
  return cstring;
}
