#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <math.h>
#include <unistd.h>
#include "sys/time.h"
#define WORKTAG 1
#define DIETAG 2

int main(int argc, char *argv[]){    
    int myid, numprocs, nb_ligne, nb_col, nb_pas;
    double td, h;
    //Initialisation MPI 
    MPI_Init(&argc,&argv);
    MPI_Comm_size(MPI_COMM_WORLD,&numprocs);
    MPI_Comm_rank(MPI_COMM_WORLD,&myid);
	
	nb_ligne = atoi(argv[1]);
    nb_col = atoi(argv[2]);
    nb_pas = atoi(argv[3]);
	td = atof(argv[4]);
	h = atof(argv[5]);
	
	if(myid == 0){ //master
		printf("Start Master \n");
		fflush(stdout);
		int x, i, j, rank;
		//Initialisation matrice
		double matrix[nb_ligne][nb_col][2];
		for (i = 0; i < nb_ligne; i++) {
			for (j = 0; j < nb_col; j++){
				matrix[i][j][0] = i*(nb_ligne-i-1)*j*(nb_col-j-1);
			}
		}
		
		rank = 1;
		MPI_Request requete[(nb_ligne-2)*(nb_col-2)];
		MPI_Status statusList[(nb_ligne-2)*(nb_col-2)];
		
		//Initialisation du chronometre
		double timeStart, timeEnd, Texec;
		struct timeval tp;
		gettimeofday (&tp, NULL); // Debut du chronometre
		timeStart = (double) (tp.tv_sec) + (double) (tp.tv_usec) / 1e6;
		
		
		for(x = 1; x <= nb_pas; x++){
			int nbSend = 0;
			for (i = 1; i < nb_ligne-1; i++) {
				for (j = 1; j < nb_col-1; j++){
					double info[5];
					info[0] = matrix[i][j][(x+1)%2]; //Cell to calculate
					info[1] = matrix[i-1][j][(x+1)%2]; //Top cell
					info[2] = matrix[i][j+1][(x+1)%2]; //Right cell
					info[3] = matrix[i+1][j][(x+1)%2]; //Bottom cell
					info[4] = matrix[i][j-1][(x+1)%2]; //Left cell
					//printf("send to rank %d \n", rank);
					MPI_Send(info,5,MPI_DOUBLE,rank,WORKTAG,MPI_COMM_WORLD);					
					MPI_Irecv(&matrix[i][j][x%2],1,MPI_DOUBLE,rank,MPI_ANY_TAG,MPI_COMM_WORLD,&requete[nbSend]);
					nbSend++;
					rank = (rank%(numprocs-1))+1;
				}
			}
			//printf("nb of send : %d \n", nbSend);
			MPI_Waitall(nbSend, requete, statusList);
			//printf("end iteration %d \n", x);
		}
		for (rank = 1; rank < numprocs; ++rank) {
                MPI_Send(0, 0, MPI_INT, rank, DIETAG, MPI_COMM_WORLD);
        }
		
		gettimeofday (&tp, NULL); // Fin du chronometre
        timeEnd = (double) (tp.tv_sec) + (double) (tp.tv_usec) / 1e6;
        Texec = timeEnd - timeStart; //Temps d'execution en secondes
        printf("durée d'exécution : %lf\n", Texec);
		
		//Affichage de la matrice de fin
		if(nb_pas%2 == 1){
			printf("Matrice obtenue parallele: \n");
			for (i = 0; i < nb_ligne; i++) {
				for (j = 0; j < nb_col; j++)     printf("%.2f    ", matrix[i][j][1]);
					printf("\n");
				}
		}
		else{
			printf("Matrice obtenue parallele: \n");
			for (i = 0; i < nb_ligne; i++) {
				for (j = 0; j < nb_col; j++)     printf("%.2f    ", matrix[i][j][0]);
					printf("\n");
				}
		}
	}
	else{ //slave
	while(1){
			double info[5];
			double result;
			MPI_Status status;
			MPI_Recv(info,5,MPI_DOUBLE,0,MPI_ANY_TAG,MPI_COMM_WORLD,&status);
			if (status.MPI_TAG == DIETAG) {
                        break;
                }			
			
			//printf(" Process %d Info = %.2f , %.2f , %.2f , %.2f, %.2f: \n", myid, info[0], info[1], info[2], info[3], info[4]);
			result = ((1-(4*td)/(h*h))*info[0])+((td/(h*h))*(info[1]+info[2]+info[3]+info[4]));
			usleep(1000);
			MPI_Send(&result,1,MPI_DOUBLE,0,WORKTAG,MPI_COMM_WORLD);
		}
	}

	MPI_Finalize();

    return 0;
}