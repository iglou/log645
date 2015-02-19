PROG = tp3
CFLAGS = -g -fPIC -m64 -Wall
LFLAGS= -fPIC -m64 -Wall
CC = mpicc

all: $(PROG)

tp3.o: tp3.c
	$(CC) $(CFLAGS) -c tp3.c

tp3 : tp3.o
	$(CC) $(LFLAGS) tp3.o -o tp3

run:
	mpirun --hostfile hostfile -np 24 tp3

ps:
	ps -fu $$USER

clean:
	/bin/rm -f *~
	/bin/rm -f *.o

test-mpi:
	mpirun --hostfile hostfile -np 4 tp3
