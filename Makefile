a.out : scheme.c object.c parser.c
	gcc -Wall -g -o $@ $^ -lreadline
