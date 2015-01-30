.PHONY: test main play
main:
	gcc -g -std=gnu99 shelstring.c -o main
test:
	gcc -g -std=gnu99 test.c -o test

play:
	gcc -g -std=gnu99 endianness-test.c -o play
