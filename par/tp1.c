#include <stdio.h>
#include "mpi.h"

void main (int argc, char *argv[]){
	int myid, numprocs;
    FILE *f1;
    int i;
 
    MPI_Init(&argc,&argv);
    MPI_Comm_size(MPI_COMM_WORLD,&numprocs);
    MPI_Comm_rank(MPI_COMM_WORLD,&myid);
 

        /* print out my rank and this run's PE size*/
    printf("Hello from %d\n",myid);
    printf("Numprocs is %d\n",numprocs);

    MPI_Finalize();
}