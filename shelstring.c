/* Format: length-prefixed string
 * length is a series of 7-bit peices, with the LSBit being an indicator bit, indicating whether the next byte is part of the length
 * EG: 0000 0100|0100 1000 0100 1001 is the string "HI" in this format
 *     -length--|------string-------
 *
 * EG: 0000 0001 0000 0000|01010011 01101000 01100101 01101100.....01110110 01100001 01100011 01110101 
 *     ------length-------|--------------  string of length 128 (the 'meat') -------------------------
 *
 * The length is LSB first - endianness determined by coin flip
 * This is a library to implement said string format, with equivelent functions to that of C strings
 */
#pragma once
#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif
//#define DEBUG 1

#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "shelstring.h"
#ifdef DEBUG
# include <stdio.h>
#endif

#define MAX_SIZES_LENGTH 4

//2^(7*n)-1
const long max_sizes[MAX_SIZES_LENGTH] = {127,16383,2097151,268435455};

short shelint_length_from_cint(unsigned int cint){
  short length_length = 1;
  while(max_sizes[length_length-1] < cint){
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

char * shelstr_meat(shelstring s){
  while(*s & 0x01)
    s++;
  //s now points to the last byte of the length, increment one more to point to the meat.
  s++;
  return s;
}

//Also returns a pointer to the meat of the shelstring
unsigned char * cint_to_shelint(unsigned int cint, int length_length, shelstring ptr){
#ifdef DEBUG
  unsigned int orig_cint = cint;
#endif
#ifdef LITTLE_ENDIAN
  //puts("all is well!");
  int i;
  for(i=0;i<length_length;i++){
    cint = cint << 1;
    ptr[i] = ((&cint)[i] | 0x01);
# ifdef DEBUG
    printf("in for loop: %02X %02X\n",(&cint)[i],ptr[i]);
# endif
  }
  ptr[i-1] &= 0xFE; //Zero the continue bit on the last byte
# ifdef DEBUG
  printf("cint_to_shelint(%d,%d,%02X) = %02X %02X\ni is %d\n", orig_cint, length_length, ptr, ptr[0], ptr[1],i);
# endif
#else
#error "Big endian not implemented... yet"
#endif
  return ptr + i;
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

size_t shelstrlen(shelstring s){
  size_t cint = 0;
  shelstrlen_ptr(s, &cint);
  return cint;
}

unsigned char * shelstrlen_ptr(shelstring shelstring, size_t * cint_i){
  unsigned char * cint_ptr = (unsigned char *)cint_i;
  int i;
  if(!(shelstring[0] & 0x01)){
#ifdef LITTLE_ENDIAN
    cint_ptr[0] = shelstring[0] >> 1;
#elif BIG_ENDIAN
    cint_ptr[sizeof(cint)] = shelstring[0] >> 1;
#endif
  }else{ //TODO: Fix this algoritm for lengths of lengths > 7 (or maybe 8 works?)
    i = -1;
    do{
      i++;
#ifdef LITTLE_ENDIAN
      (cint_ptr)[i] =
#elif BIG_ENDIAN // No idea if big endian works, beware
	(cint_ptr)[sizeof(cint) - i] = //TODO: test if this works
#endif
	(shelstring[i] >> i+1) |
	((shelstring[i] & 0x01) ?
	 (shelstring[i+1] & 0xFE) << 6-i // `& 0xFE` is to eliminate the continue bit
	 : 0x00);
    }while(shelstring[i] & 0x01);
  }
  return shelstring + i;
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
  while(shelstring[0] & 0x01){
#ifdef DEBUG
    printf("%X is %02X\n",shelstring, shelstring[0]);
#endif
    shelstring++;
  }
  //shelstring now points to the last byte of the length, so increment one more to get to the juicy characters
  for(int i=0;i<*length_out;i++){
#ifdef DEBUG
    printf("%02X\n",shelstring[i]);
#endif
    chars[i] = shelstring[i];
  }
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


int shelstrcasecmp(shelstring s1, shelstring s2){
  //First, compare the length parts of the strings
  int i = 0;
  while(s1[i] == s2[i]){
    if(!(s1[i] & 0x01)) //If we have reached the end of the length prefix
      goto stuff;
    i++;
  }
  //If we've reached this point, then the length portions don't match
  return -i;
 stuff:; //At this point, the lengths do match, compare the rest
  int len = shelstrlen(s1)+i;
  for(;i<len;i++){
    if(!(tolower(s1[i]) == tolower(s2[i])))
      return i;
  }
  return 0;
}

shelstring shelstrcpy(shelstring source, unsigned char * dest){
  int i = 0;
  dest[i] = source[i];
  while(0x01 & source[i]){
    i++;
    dest[i] = source[i];
  }
  size_t len = shelstrlen(source)+i;
  for(;i<len;i++){
    dest[i] = source[i];
  }
}

shelstring shelstrcat(shelstring s1, shelstring s2){
  size_t s1_len = shelstrlen(s1);
  size_t s2_len = shelstrlen(s2);
  size_t total_len = s1_len + s2_len;
  size_t length_length = shelint_length_from_cint(total_len);
  shelstring res;
  if(!(res = malloc(length_length + total_len)))
    return NULL;
  unsigned char * meat = cint_to_shelint(total_len,length_length,res);
  //This next part could be multi-threaded technically, but it probably takes more
  // cycles to start & manage threads than it would save.
  do
    s1++;
  while(*s1 & 0x01);
  s1++;
  do
    s2++;
  while(*s2 & 0x01);
  s2++;
  int i=0;
  while(i<s1_len){
    meat[i] = *s1;
    s1++;
    i++;
  }
  while(i<s2_len+s1_len){
    meat[i] = *s2;
    s2++;
    i++;
  }
  return res;
}

int shelstrcmp(shelstring s1, shelstring s2){
  //First, compare the length parts of the strings
  int i = 0;
  while(s1[i] == s2[i]){
    if(!(s1[i] & 0x01)) //If we have reached the end of the length prefix
      goto stuff;
    i++;
  }
  //If we've reached this point, then the length portions don't match
  return -i;
 stuff:; //At this point, the lengths do match, compare the rest
  int len = shelstrlen(s1)+i;
  for(;i<len;i++){
    if(!(s1[i] == s2[i]))
      return i;
  }
  return 0;
}

char * shelstrchr(shelstring s, char c){
  size_t len = 0;
  char * meat = shelstrlen_ptr(s,&len);
  return memchr(meat,c,len);
}

char * shelstrrchr(shelstring s, char c){
  size_t len = 0;
  char * meat = shelstrlen_ptr(s,&len);
  return memrchr(meat,c,len);
}

char * shelstrstr(shelstring source, shelstring sub){ //Who names these!?
  /* Hey guise, we need to have a function that finds a string within a string,
   * what should we call it?
   *
   * "find_substring"? Nope.
   * "find_str"? Nope.
   * "substr"? It would've been odd, but reasonable. Nope.
   * No, someone had a few, then typed "str" twice on accident.
   * It's the only reasonable explanation. Maybe they submitted it as well,
   * and then said it was on purpose to hide their embarrassment.
   * Try not to do that people. It's bad. I say try because I did it just today
   * but it didn't affect standard C library naming conventions, so it wasn't
   * as bad. */
  size_t source_len = 0; //"find_substring" was shortened to "strstr"
  //But I still have to write out "unsigned" (but not "integer")
  char * source_meat = shelstrlen_ptr(source, &source_len);
  //Hey past self, if you had to guess, do you think you'll ever
  //use the variable name 'source_meat'?

  //Huh? not unless I'm make some meat processing program

  //kk
  size_t sub_len = 0; //Yes yes I realize it's not exactly DRY shutup.
  char * sub_meat = shelstrlen_ptr(sub, &sub_len);
  char * final_ptr = source_meat+(source_len-sub_len);
  for(;source_meat<final_ptr;source_meat++){
    if(memcmp(source_meat,sub_meat,sub_len) == 0)
      return source_meat;
  }
  return NULL;
}
