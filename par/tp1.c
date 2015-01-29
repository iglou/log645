#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <math.h>
 
/************************************************************
This is a simple send/receive program in MPI
************************************************************/

int main(argc,argv)
int argc;
char *argv[];
{
    int myid, numprocs, tag;
    int probleme, start_value, nb_iteration, x, i, j;
    MPI_Status status;
 
    MPI_Init(&argc,&argv);
    MPI_Comm_size(MPI_COMM_WORLD,&numprocs);
    MPI_Comm_rank(MPI_COMM_WORLD,&myid);
    tag=1234;
    probleme = atoi(argc[1]);
    start_value = atoi(argv[2]);
    nb_iteration = atoi(argv[3]);

    if(myid < 8){
        int array[8];
        for (i = 0; i < 8; i++) array[i] = start_value;

        for (x = 0; x <= nb_iteration; x++)
        {
            for (i = 0; i < 8; i++) {
                usleep(1000);
                array[i] = array[i] + (i + myid)*x;
            }
        }
        MPI_Send(&array,8,MPI_INT,8,tag,MPI_COMM_WORLD);
        //printf("processor %d  sent %d\n",myid,array[0]);
    }
    if(myid == 8){
        int matrix[8][8];
        for (i = 0; i < 8; i++){
            MPI_Recv(matrix[i],8,MPI_INT,i,tag,MPI_COMM_WORLD,&status);
            //printf("processor %d  got %d\n",myid,matrix[i][0]);
        }

        for (i = 0; i < 8; i++) {
            for (j = 0; j < 8; j++) {
                printf("%d", matrix[i][j]);
                printf("    ");
            }
            printf("\n");
        }

    }
    MPI_Finalize();
}