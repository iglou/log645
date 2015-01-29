#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <math.h>
#include <unistd.h>
#include "sys/time.h"

int main(int argc, char *argv[]){
    //Initialisation du chronometre
    double timeStart, timeEnd, Texec;
    struct timeval tp;
    gettimeofday (&tp, NULL); // Debut du chronometre
    timeStart = (double) (tp.tv_sec) + (double) (tp.tv_usec) / 1e6;
    
    int myid, numprocs, tag, probleme, start_value, nb_iteration, x, i;
    
    //Initialisation MPI 
    MPI_Init(&argc,&argv);
    MPI_Comm_size(MPI_COMM_WORLD,&numprocs);
    MPI_Comm_rank(MPI_COMM_WORLD,&myid);
    MPI_Status status;
   
    //Initialisation des paramètres
    tag=1;
    probleme = atoi(argv[1]);
    start_value = atoi(argv[2]);
    nb_iteration = atoi(argv[3]);

    if(myid < 8){ //Les 8 premiers processeurs font les calculs sur la matrice
        int array[8];
        //Initialisation du tableau
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
                    if(i == 0)  array[i] = array[i] + myid*x;
                    else        array[i] = array[i] + array[i-1] * x;
                }
            }
            MPI_Send(&array,8,MPI_INT,8,tag,MPI_COMM_WORLD);
        }
        else{
            printf("Numéro de problème inexistant\n");
        }
    }
    if(myid == 8){ //Le dernier processeur rassemble tous les calculs
        int matrix[8][8];
        for (i = 0; i < 8; i++){
            MPI_Recv(matrix[i],8,MPI_INT,i,tag,MPI_COMM_WORLD,&status);
        }

        //Affichage de la matrice de fin
        for (i = 0; i < 8; i++) {
            for (x = 0; x < 8; x++)     printf("%d    ", matrix[i][x]);
            printf("\n");
        }
    }

    MPI_Finalize();

    if(myid == 8){
        gettimeofday (&tp, NULL); // Fin du chronometre
        timeEnd = (double) (tp.tv_sec) + (double) (tp.tv_usec) / 1e6;
        Texec = timeEnd - timeStart; //Temps d'execution en secondes
        printf("durée d'exécution : %lf\n", Texec);
    }

    return 0;
}