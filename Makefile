OPTS=-std=gnu11
#DEPS=shelstring.h

.PHONY: test main play shelstring-check
shared-lib: shelstring.c
	gcc -c -fPIC shelstring.c -o shelstring.o
	gcc -shared -Wl,-soname,libshelstring.so.1 -o libmean.so.1.0.1  shelstring.o
static-lib: shelstring.c
	gcc $(OPTS) -c shelstring.c -o shelstring.o
	ar rcs libshelstring.a shelstring.o
test: shelstring.c shelstring.check
	checkmk shelstring.check > shelstring-check.c
	gcc $(OPTS) -lcheck -D_GNU_SOURCE shelstring-check.c -o shelstring-check
	./shelstring-check
merr: shelstring.c test.c
	gcc $(OPTS) test.c -o test
play: shelstring.c endianness-test.c
	gcc $(OPTS) endianness-test.c -o play
