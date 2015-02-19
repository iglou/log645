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
    
    int myid, numprocs, tag, j, i, nb_ligne, nb_col, nb_pas, td, h;
    
    //Initialisation MPI 
    MPI_Init(&argc,&argv);
    MPI_Comm_size(MPI_COMM_WORLD,&numprocs);
    MPI_Comm_rank(MPI_COMM_WORLD,&myid);
    MPI_Status status;
   
    //Initialisation des paramètres
    tag=1;
    nb_ligne = atoi(argv[1]);
    nb_col = atoi(argv[2]);
    nb_pas = atoi(argv[3]);
	td = atoi(argv[4]);
	h = atoi(argv[5]);
	
	//Initialisation matrice
	double matrix[nb_ligne][nb_col];
	for (i = 0; i < nb_ligne; i++) {
		for (j = 0; j < nb_col; j++){
			matrix[i][j] = i*(nb_ligne-i-1)*j*(nb_col-j-1);
		}
	}

    //Affichage de la matrice de fin
    printf("Matrice obtenue : \n");
    for (i = 0; i < nb_ligne; i++) {
        for (j = 0; j < nb_col; j++)     printf("%.2f    ", matrix[i][j]);
			printf("\n");
        }

    MPI_Finalize();

    return 0;
}