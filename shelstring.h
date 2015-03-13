#pragma once

#ifndef _GNU_SOURCE
 #define _GNU_SOURCE
#endif
#include <string.h>

typedef unsigned char* shelstring;

//From a regular integer(cint), determine what the resulting length in bytes a shelint will be.
short shelint_length_from_cint(unsigned int cint);

//Convert a regular integer(cint) to a shelint of length (length_length)
//at the address (ptr)
unsigned char * cint_to_shelint(unsigned int cint, int length_length, unsigned char *ptr);

//Convert a arbitrary set of bytes(cstring) of length (size) to a shelstring
unsigned char * chars_to_shelstring(unsigned char * cstring,size_t size);

//Convert a null-terminated string(cstring) to a shelstring
unsigned char * cstring_to_shelstring(unsigned char * cstring);

//Get the length of a shelstring, equivelent to strlen()
//Also happens to be the same as shelint to cint
size_t shelstrlen(shelstring shelstring);

//Use this when you want to avoid schlemiel the painter problems
//takes in a sheltring and an int pointer which MUST be initialized to 0
//otherwise the behaviour is undefined.
//Returns a pointer to the meat of the string.
unsigned char * shelstrlen_ptr(shelstring shelstring, size_t * cint_i);

//Convert a shelstring to just characters with a length out
unsigned char * shelstring_to_chars(shelstring shelstring,
				    unsigned int *length_out);
//This does the same as above (in face shelstring_to_chars calls this function)
//but it also mallocs an extra amount of bytes equal to addon_length
unsigned char * shelstring_to_chars_with_addon(shelstring shelstring,
					       unsigned int *length_out,
					       unsigned int addon_length );

//Convert a shelstring to a cstring(null-terminated string)
//WARNING: there is no garuntee that the shelstring does not contain nulls
//and if it does the resulting cstring will terminate early.
unsigned char * shelstring_to_cstring(shelstring shelstring);

//Past this point its the same as regular string.h functions

shelstring shelstrcpy(shelstring source, unsigned char * dest);

shelstring shelstrcat(shelstring s1, shelstring s2);

int shelstrcmp(shelstring s1, shelstring s2);

char * shelstrchr(shelstring s, char c);

char * shelstrrchr(shelstring s, char c);

char * shelstrstr(shelstring source, shelstring sub);

#include "shelstring.c"
