
#ifndef SHELINT_H
#define SHELINT_H 1

#include <string.h>

typedef unsigned char* shelstring;

//From a regular integer(cint), determine what the resulting length in bytes a shelint will be.
short shelint_length_from_cint(unsigned int cint);

//Convert a regular integer(cint) to a shelint of length (length_length)
//at the address (ptr)
void cint_to_shelint(unsigned int cint, int length_length, unsigned char *ptr);

//Convert a arbitrary set of bytes(cstring) of length (size) to a shelstring
unsigned char * chars_to_shelstring(unsigned char * cstring,size_t size);

//Convert a null-terminated string(cstring) to a shelstring
unsigned char * cstring_to_shelstring(unsigned char * cstring);

//Get the length of a shelstring, equivelen to strlen()
//Also happens to be the same as shelint to cint
unsigned int shelstrlen(shelstring shelstring);

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

#include "shelstring.c"
#endif
