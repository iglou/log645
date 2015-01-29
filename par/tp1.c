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
    double timeStart, timeEnd, Texec;
    struct timeval tp;
    gettimeofday (&tp, NULL); // Debut du chronometre
        timeStart = (double) (tp.tv_sec) + (double) (tp.tv_usec) / 1e6;

    int myid, numprocs, tag;
    int probleme, start_value, nb_iteration, x, i, j;
    MPI_Status status;
 
    MPI_Init(&argc,&argv);
    MPI_Comm_size(MPI_COMM_WORLD,&numprocs);
    MPI_Comm_rank(MPI_COMM_WORLD,&myid);
    tag=1234;
    probleme = atoi(argv[1]);
    start_value = atoi(argv[2]);
    nb_iteration = atoi(argv[3]);

    

    if(myid < 8){ 
        int array[8];
        for (i = 0; i < 8; i++) array[i] = start_value;
       
        if(probleme == 1){
            for (x = 0; x <= nb_iteration; x++)
            {
                for (i = 0; i < 8; i++) {
                    usleep(1000);
                    array[i] = array[i] + (i + myid)*x;
                }
            }
            MPI_Send(&array,8,MPI_INT,8,tag,MPI_COMM_WORLD);
        }
        else if(probleme == 2){
            for (x = 0; x <= nb_iteration; x++)
            {
                for (i = 0; i < 8; i++) {
                    usleep(1000);
                    if(i == 0){
                        array[i] = array[i] + myid*x;
                    }
                    else{
                        array[i] = array[i] + array[i-1] * x;
                    }
                }
            }
            MPI_Send(&array,8,MPI_INT,8,tag,MPI_COMM_WORLD);
        }
    }
    if(myid == 8){
        int matrix[8][8];
        for (i = 0; i < 8; i++){
            MPI_Recv(matrix[i],8,MPI_INT,i,tag,MPI_COMM_WORLD,&status);
        }

        for (i = 0; i < 8; i++) {
            for (j = 0; j < 8; j++) {
                printf("%d", matrix[i][j]);
                printf("    ");
            }
            printf("\n");
        }
        gettimeofday (&tp, NULL); // Fin du chronometre
        timeEnd = (double) (tp.tv_sec) + (double) (tp.tv_usec) / 1e6;
        Texec = timeEnd - timeStart; //Temps d'execution en secondes

        printf("durée d'exécution de %d : %lf\n", myid, Texec);

    }

    MPI_Finalize();
    gettimeofday (&tp, NULL); // Fin du chronometre
    timeEnd = (double) (tp.tv_sec) + (double) (tp.tv_usec) / 1e6;
    Texec = timeEnd - timeStart; //Temps d'execution en secondes

    printf("durée d'exécution de %d : %lf\n",myid, Texec);
}