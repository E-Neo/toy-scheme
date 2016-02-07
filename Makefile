a.out : test.c object.c parser.c
	gcc -Wall -g -o $@ $^
