a.out : test.c scheme.c parser.c
	gcc -Wall -g -o $@ $^
