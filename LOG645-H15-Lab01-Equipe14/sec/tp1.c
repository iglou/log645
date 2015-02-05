#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "sys/time.h"


int main (int argc, char *argv[]){	
	//Initialisation du chronometre
	double timeStart, timeEnd, Texec;
	struct timeval tp;
	
	int probleme, start_value, nb_iteration, x, i, j, maxtrix[8][8];
	
	//Initialisation des paramètres
	probleme = atoi(argv[1]);
    start_value = atoi(argv[2]);
    nb_iteration = atoi(argv[3]);

    printf("probleme : %d\n", probleme);
    printf("valeur de départ de la matrice : %d\n", start_value);
    printf("nombre d'iteration : %d\n", nb_iteration);

	//Initialisation de la matrice
	for (i = 0; i < 8; i++) {
		for (j = 0; j < 8; j++) maxtrix[i][j] = start_value;
	}
	
	if(probleme == 1)
	{
		gettimeofday (&tp, NULL); // Debut du chronometre
		timeStart = (double) (tp.tv_sec) + (double) (tp.tv_usec) / 1e6;

		for (x = 0; x <= nb_iteration; x++)
		{
			for (i = 0; i < 8; i++) {
				for (j = 0; j < 8; j++) {
					usleep(1000);
					maxtrix[i][j] = maxtrix[i][j] + (i + j)*x;
				}
			}
		}
	}
	else if(probleme == 2){
		gettimeofday (&tp, NULL); // Debut du chronometre
		timeStart = (double) (tp.tv_sec) + (double) (tp.tv_usec) / 1e6;
		
		for (x = 0; x <= nb_iteration; x++)
		{
			for (i = 0; i < 8; i++) {
				for (j = 0; j < 8; j++)
				{
					usleep(1000);
					if (j == 0)	maxtrix[i][j] = maxtrix[i][j] + (i*x);
					else		maxtrix[i][j] = maxtrix[i][j] + maxtrix[i][j - 1] * x;
				}
			}
		}
	}
	else{
		printf("Numéro de problème inexistant\n");
	}
	//Affichage de la matrice de fin
	printf("Matrice obtenue : \n");
	for (i = 0; i < 8; i++) {
		for (j = 0; j < 8; j++)		printf("%d    ",maxtrix[i][j]);
		printf("\n");
	}

	gettimeofday (&tp, NULL); // Fin du chronometre
	timeEnd = (double) (tp.tv_sec) + (double) (tp.tv_usec) / 1e6;
	Texec = timeEnd - timeStart; //Temps d'execution en secondes

	printf("durée d'exécution : %lf\n", Texec);
	return 0;
}