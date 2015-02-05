#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include "sys/time.h"

int main(int argc, char *argv[]){
    //Initialisation du chronometre
    //double timeStart, timeEnd, Texec;
    //struct timeval tp;
    //gettimeofday (&tp, NULL); // Debut du chronometre
    //timeStart = (double) (tp.tv_sec) + (double) (tp.tv_usec) / 1e6;
    
    int myid, numprocs, tag, probleme, start_value, nb_iteration, x, i, maxtrix[10][10];
   
    //Initialisation des paramètres
    probleme = atoi(argv[1]);
    start_value = atoi(argv[2]);
    nb_iteration = atoi(argv[3]);
	
	//Initialisation de la matrice
	for (i = 0; i < 8; i++) {
		for (j = 0; j < 8; j++) maxtrix[i][j] = start_value;
	}
	
    if(probleme == 1){
		
	}
	else if(probleme == 2){
		
	}

    return 0;
}

void spinWait(int milliseconds){
	struct timeval startTime;
	struct timeval endTime;
	gettimeofday(&startTime, NULL);
	do
	{
		gettimeofday(&endTime, NULL);
	} 
	while ((endTime.tv_sec - startTime.tv_sec) * 1000000 + (endTime.tv_usec - startTime.tv_usec) < milliseconds * 1000);
	return;
}