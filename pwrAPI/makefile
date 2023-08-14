all: dummy.c
	gcc -std=c99 -g -Wall -o pwrAPINodeExample dummy.c  -L../build-lassen/variorum  -lvariorum -ljansson -D_GNU_SOURCE

clean:
	$(RM) pwrAPINodeExample
