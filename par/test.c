#include <stdio.h>
#include <stdlib.h>
#include "sys/time.h"
#include "mpi.h"

void main (int argc, char *argv[]){
	int myid, numprocs;
    int tag = 3;
 
    MPI_Init(&argc,&argv);
    MPI_Comm_size(MPI_COMM_WORLD,&numprocs);
    MPI_Comm_rank(MPI_COMM_WORLD,&myid);
    MPI_Status status;

        if(myid = 0){
            int data = 3;
            printf("je suis le processus %d\n", myid);
            MPI_Send(&data,1,MPI_INT,2,myid,MPI_COMM_WORLD);
        }        

        if(myid == 2){
            int receive;
            printf("je suis le processus %d\n", myid);
            MPI_Recv(&receive, 1, MPI_INT, 0, tag, MPI_COMM_WORLD, &status);
        }

    printf("Numprocs is %d\n",numprocs);

    MPI_Finalize();
}