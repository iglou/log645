#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <math.h>
#include <unistd.h>
#include "sys/time.h"
#define WORKTAG 1
#define DIETAG 2



int main(int argc, char *argv[]){    
    int myid, numprocs, nb_ligne, nb_col, nb_pas, td, h;
    
    //Initialisation MPI 
    MPI_Init(&argc,&argv);
    MPI_Comm_size(MPI_COMM_WORLD,&numprocs);
    MPI_Comm_rank(MPI_COMM_WORLD,&myid);
	
	printf("%d\n", argc);
	fflush(stdout);
	
	nb_ligne = atoi(argv[1]);
    nb_col = atoi(argv[2]);
    nb_pas = atoi(argv[3]);
	td = atoi(argv[4]);
	h = atoi(argv[5]);
	
	double matrix[nb_ligne][nb_col][2];
	
	if(myid == 0){ //master
		printf("Start Master \n");
		fflush(stdout);
		int x, i, j, rank;
		int done = 0;
		//INFORMATION this_info;
		//Initialisation matrice
		double matrix[nb_ligne][nb_col][2];
		for (i = 0; i < nb_ligne; i++) {
			for (j = 0; j < nb_col; j++){
				matrix[i][j][0] = i*(nb_ligne-i-1)*j*(nb_col-j-1);
			}
		}
		
		rank = 1;
		MPI_Request requete[nb_ligne*nb_col];
		MPI_Status statusList[nb_col*nb_ligne];
		for(x = 1; x <= nb_pas; x++){
			for (i = 0; i < nb_ligne; i++) {
				for (j = 0; j < nb_col; j++){
					int info[3];
					info[0] = i;
					info[1] = j;
					info[2] = x;
					if (rank >= numprocs){
						rank = 1;
						//printf("send to rank %d \n", rank);
						MPI_Send(info,         //* message buffer 
						3,              		//* one data item 
						MPI_INT,        		//* data item is an integer 
						rank,           		//* destination process rank 
						WORKTAG,        				//* user chosen message tag 
						MPI_COMM_WORLD);		//* always use this 
					}
					else{
						//printf("send to rank %d \n", rank);
						MPI_Send(info,         //* message buffer 
						3,              		//* one data item 
						MPI_INT,        		//* data item is an integer 
						rank,           		//* destination process rank 
						WORKTAG,        				//* user chosen message tag 
						MPI_COMM_WORLD);		//* always use this 
						
						rank ++;
						//rank%(numprocs-1)+1
					}
					printf("test send \n");
					fflush(stdout);
					//MPI_IRecv(&matrix[i][j][(x+1)%2],1,MPI_INT,rank,MPI_ANY_TAG,MPI_COMM_WORLD,&status, &requete[(i+j)*nb_col]);
				}
			}
			//MPI_Waitall(nb_col*nb_ligne, requete, statusList);
			printf("end iteration %d \n", x);
		}
		for (rank = 1; rank < numprocs; ++rank) {
                MPI_Send(0, 0, MPI_INT, rank, DIETAG, MPI_COMM_WORLD);
        }
	}
	else{ //slave
	printf("enter slave \n");
	fflush(stdout);
	while(1){
			int info[3];
			
			MPI_Status status;
			MPI_Recv(info,3,MPI_INT,0,MPI_ANY_TAG,MPI_COMM_WORLD,&status);
			printf("test recv \n");
			fflush(stdout);
			if (status.MPI_TAG == DIETAG) {
                        break;
                }			

			printf(" Process %d Info = %d , %d , %d : \n", myid, info[0], info[1], info[2]);
		}
	}
	MPI_Finalize();

    return 0;
}