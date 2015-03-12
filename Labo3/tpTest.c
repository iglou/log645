#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <math.h>
#include <unistd.h>
#include "sys/time.h"
#define TAG_NOTOVER 0
#define TAG_OVER 1

struct info {
                 int ligne;
                 int col;
                 int ite;
              };

typedef struct info INFORMATION;

int main(int argc, char *argv[]){    
    int myid, numprocs, nb_ligne, nb_col, nb_pas, td, h;
    double matrix[nb_ligne][nb_col][2];
	
    //Initialisation MPI 
    MPI_Init(&argc,&argv);
    MPI_Comm_size(MPI_COMM_WORLD,&numprocs);
    MPI_Comm_rank(MPI_COMM_WORLD,&myid);
	
	nb_ligne = atoi(argv[1]);
    nb_col = atoi(argv[2]);
    nb_pas = atoi(argv[3]);
	td = atoi(argv[4]);
	h = atoi(argv[5]);

	if(myid == 0){ //master
		int x, i, j, rank;
		int done = 0;
		INFORMATION this_info;
		//Initialisation matrice
		double matrix[nb_ligne][nb_col][2];
		for (i = 0; i < nb_ligne; i++) {
			for (j = 0; j < nb_col; j++){
				matrix[i][j][0] = i*(nb_ligne-i-1)*j*(nb_col-j-1);
			}
		}
		rank = 1;
		for(x = 1; x <= nb_pas; x++){
			for (i = 0; i < nb_ligne; i++) {
				for (j = 0; j < nb_col; j++){
					int info[3];
					info[0] = i;
					info[1] = j;
					info[2] = x;
					MPI_Send(&info,         /* message buffer */
					3,              /* one data item */
					MPI_INT,        /* data item is an integer */
					rank,           /* destination process rank */
					1,        /* user chosen message tag */
					MPI_COMM_WORLD);/* always use this */
						rank%(numprocs-1)+1
				}
			}
			while (done == 0){
				
				
				done = 1;
			}
		}
	}
	MPI_Finalize();

    return 0;
}