PROG = hello
CFLAGS = -g -fPIC -m64 -Wall
LFLAGS= -fPIC -m64 -Wall
CC = mpicc

all: $(PROG)

hello.o: hello.c
	$(CC) $(CFLAGS) -c hello.c

hello : hello.o
	$(CC) $(LFLAGS) hello.o -o hello

run:
	mpirun --hostfile hostfile -np 24 hello

ps:
	ps -fu $$USER

clean:
	/bin/rm -f *~
	/bin/rm -f *.o

test-mpi:
	mpirun --hostfile hostfile -np 4 hello

