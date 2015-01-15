# build an executable named myprog from myprog.c
  all: test.c
	  gcc -g -Wall -o test test.c

  clean:
  	$(RM) test