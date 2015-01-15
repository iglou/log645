#include <stdio.h>

void main (int argc, char *argv[]){
	int c;
	int p;
	int n;
	int i, j;
	printf("Choix du probleme : \n");
	scanf("%d", &c);
	printf("Valeur de déparrt de la matrice 0 : \n");
	scanf("%d", &p);
	printf("Nombre d'altération : \n");
	scanf("%d", &n);
	printf("%d", c);
	printf("\n");
	printf("%d", p);
	printf("\n");
	printf("%d", n);
	printf("\n");
	
	if(c == 1)
	{
		int maxtrix[8][8];
		for (i = 0; i < 8; i++) {
			for (j = 0; j < 8; j++) maxtrix[i][j] = p;
		}

		int var_k = n;
		for (i = 0; i < var_k; i++)
		{
			for (i = 0; i < 8; i++) {
				for (j = 0; j < 8; j++) {
					usleep(1000);
					maxtrix[i][j] = maxtrix[i][j] + (i + j)*var_k;
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
		int maxtrix[8][8];
		for (i = 0; i < 8; i++) {
			for (j = 0; j < 8; j++) maxtrix[i][j] = 3;
		}

		int var_k = n;
		for (i = 0; i < var_k; i++)
		{
			for (i = 0; i < 8; i++) {
				for (j = 0; j < 8; j++)
				{
					if (j == 0)
					{
						usleep(1000);
						maxtrix[i][j] = maxtrix[i][j] + (i*var_k);
					}
					else
					{
						usleep(1000);
						maxtrix[i][j] = maxtrix[i][j] + maxtrix[i][j - 1] * var_k;
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
}