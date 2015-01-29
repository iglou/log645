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
    int myid, numprocs;
    int tag,source,destination,count;
    int buffer;
    int p, n, x, i;
    MPI_Status status;
 
    MPI_Init(&argc,&argv);
    MPI_Comm_size(MPI_COMM_WORLD,&numprocs);
    MPI_Comm_rank(MPI_COMM_WORLD,&myid);
    tag=1234;
    source=0;
    destination=1;
    count=1;
    p=3;
    n=5;

    if(myid < 8){
        //int *array = (int*)malloc(8*sizeof(int));
        int array[8];
        for (i = 0; i < 8; i++) array[i] = p;

        for (x = 0; x <= n; x++)
        {
            for (i = 0; i < 8; i++) {
                //usleep(1000);
                array[i] = array[i] + (i + myid)*x;
            }
        }

        int send=array[0];

        MPI_Send(array,count,MPI_INT,8,tag,MPI_COMM_WORLD);
        printf("processor %d  sent %d\n",myid,send);
    }
    if(myid == 8){
        //int matrix[8][8];
        for (i = 0; i < 8; i++){
            MPI_Recv(&buffer,count,MPI_INT,i,tag,MPI_COMM_WORLD,&status);
            printf("processor %d  got %d\n",myid,buffer);
        }

    }
    MPI_Finalize();
}