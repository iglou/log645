#include <stdio.h>
#include <stdlib.h>
#include "sys/time.h"
#include "mpi.h"

void main (int argc, char *argv[]){
	int myid, numprocs;
    int choix = atoi (argv[1]);
    int p = atoi(argv[2]);
    int n = atoi(argv[3]);
    int x, i, j;
    int var_k = n;

    int array[8];
    for(i = 0; i < 8; i++) {array[i] = p;}
 
    MPI_Init(&argc,&argv);
    MPI_Comm_size(MPI_COMM_WORLD,&numprocs);
    MPI_Comm_rank(MPI_COMM_WORLD,&myid);
    MPI_Status status;

    if(choix == 1){

        if(myid < 8){
            for (x = 0; x <= var_k; x++)
            {
                for (i = 0; i < 8; i++) {
                    usleep(1000);
                    array[i] = array[i] + (i + myid)*x;
                }
            }
            printf("je suis le processus %d\n", j);
            MPI_Send(&array,1,MPI_INT,8,j,MPI_COMM_WORLD);
        }        

        if(myid == 8){
            int matrix[8][8];
            int matrix2[8][8];
            int root;

            MPI_Gather(matrix, 8, MPI_INT, matrix2, 8, MPI_INT, root, MPI_COMM_WORLD);

            printf("matrix1\n");
            for (i = 0; i < 8; i++) {
                for (j = 0; j < 8; j++) {
                    printf("%d", maxtrix[i][j]);
                    printf("    ");
                }
                printf("\n");
            }
        }
    }

    printf("Numprocs is %d\n",numprocs);

    MPI_Finalize();
}