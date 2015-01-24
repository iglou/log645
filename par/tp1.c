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
    int tag,count, i;
    int buffer;
    MPI_Status status;
 
    MPI_Init(&argc,&argv);
    MPI_Comm_size(MPI_COMM_WORLD,&numprocs);
    MPI_Comm_rank(MPI_COMM_WORLD,&myid);
    tag=1234;
    count=1;

    if(myid < 8){
        int x;
        int array[8];
        for(i = 0; i < 8; i++) {array[i] = p;}
        
        for (x = 0; x <= var_k; x++){
            for (i = 0; i < 8; i++) {
                usleep(1000);
                array[i] = array[i] + (i + myid)*x;
            }
        }

        buffer=array[7];
        MPI_Send(&buffer,count,MPI_INT,8,tag,MPI_COMM_WORLD);
        printf("processor %d  sent %d\n",myid,buffer);
    }

    if(myid == 8){
        int matrix[8][8], j;
        
        MPI_Recv(&buffer,count,MPI_INT,0,tag,MPI_COMM_WORLD,&status);
        printf("processor %d  got %d\n",myid,buffer);
        matrix[7][0]=buffer;

        MPI_Recv(&buffer,count,MPI_INT,1,tag,MPI_COMM_WORLD,&status);
        printf("processor %d  got %d\n",myid,buffer);
        matrix[7][1]=buffer;

        MPI_Recv(&buffer,count,MPI_INT,2,tag,MPI_COMM_WORLD,&status);
        printf("processor %d  got %d\n",myid,buffer);
        matrix[7][2]=buffer;

        MPI_Recv(&buffer,count,MPI_INT,3,tag,MPI_COMM_WORLD,&status);
        printf("processor %d  got %d\n",myid,buffer);
        matrix[7][3]=buffer;

        MPI_Recv(&buffer,count,MPI_INT,4,tag,MPI_COMM_WORLD,&status);
        printf("processor %d  got %d\n",myid,buffer);
        matrix[7][4]=buffer;

        MPI_Recv(&buffer,count,MPI_INT,5,tag,MPI_COMM_WORLD,&status);
        printf("processor %d  got %d\n",myid,buffer);
        matrix[7][5]=buffer;

        MPI_Recv(&buffer,count,MPI_INT,6,tag,MPI_COMM_WORLD,&status);
        printf("processor %d  got %d\n",myid,buffer);
        matrix[7][6]=buffer;

        MPI_Recv(&buffer,count,MPI_INT,7,tag,MPI_COMM_WORLD,&status);
        printf("processor %d  got %d\n",myid,buffer);
        matrix[7][7]=buffer;

        for (i = 0; i < 8; i++) {
            for (j = 0; j < 8; j++) {
                printf("%d", maxtrix[i][j]);
                printf("    ");
            }
            printf("\n");
        }
    }
    MPI_Finalize();
}