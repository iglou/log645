#include <stdio.h>
#include "sys/time.h"

void main (int argc, char *argv[]){	
	double timeStart, timeEnd, Texec;
	struct timeval tp;
	
	int choix;
	int p;
	int n;
	int x, i, j;
	printf("Choix du probleme : \n");
	scanf("%d", &choix);
	printf("Valeur de départ de la matrice 0 : \n");
	scanf("%d", &p);
	printf("Nombre d'altération : \n");
	scanf("%d", &n);
	
	if(choix == 1)
	{
		gettimeofday (&tp, NULL); // Debut du chronometre
		timeStart = (double) (tp.tv_sec) + (double) (tp.tv_usec) / 1e6;
		int maxtrix[8][8];
		for (i = 0; i < 8; i++) {
			for (j = 0; j < 8; j++) maxtrix[i][j] = p;
		}

		int var_k = n;
		for (x = 0; x <= var_k; x++)
		{
			for (i = 0; i < 8; i++) {
				for (j = 0; j < 8; j++) {
					usleep(1000);
					maxtrix[i][j] = maxtrix[i][j] + (i + j)*x;
				}
			}
		}

		printf("Probleme 1 !\n");

		for (i = 0; i < 8; i++) {
			for (j = 0; j < 8; j++) {
				printf("%d", maxtrix[i][j]);
				printf("    ");
			}
			printf("\n");
		}

	}
	else{
		gettimeofday (&tp, NULL); // Debut du chronometre
		timeStart = (double) (tp.tv_sec) + (double) (tp.tv_usec) / 1e6;
		int maxtrix[8][8];
		for (i = 0; i < 8; i++) {
			for (j = 0; j < 8; j++) maxtrix[i][j] = p;
		}

		int var_k = n;
		for (x = 0; x <= var_k; x++)
		{
			for (i = 0; i < 8; i++) {
				for (j = 0; j < 8; j++)
				{
					if (j == 0)
					{
						usleep(1000);
						maxtrix[i][j] = maxtrix[i][j] + (i*x);
					}
					else
					{
						usleep(1000);
						maxtrix[i][j] = maxtrix[i][j] + maxtrix[i][j - 1] * x;
					}
				}
			}
		}

		printf("Probleme 2 !\n");

		for (i = 0; i < 8; i++) {
			for (j = 0; j < 8; j++) {
				printf("%d",maxtrix[i][j]);
				printf("   ");
			}
			printf("\n");
		}
	}
	gettimeofday (&tp, NULL); // Fin du chronometre
	timeEnd = (double) (tp.tv_sec) + (double) (tp.tv_usec) / 1e6;
	Texec = timeEnd - timeStart; //Temps d'execution en secondes

	printf("durée d'exécution : %lf\n", Texec);
}