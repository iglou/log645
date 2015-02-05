#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <omp.h>
#include "sys/time.h"

#define NRA 10                 /* number of rows in matrix A */
#define NCA 10                 /* number of columns in matrix A */
#define NCB 10                  /* number of columns in matrix B */
#define start_value 3
#define nb_iteration 4

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

int main(int argc, char *argv[]){    
	double timeStart, timeEnd, Texec;
    struct timeval tp;
    gettimeofday (&tp, NULL); // Debut du chronometre
    timeStart = (double) (tp.tv_sec) + (double) (tp.tv_usec) / 1e6;
	
    int	tid, nthreads, i, j, k, chunk, x;
	
	int matrix[10][10];
	
	chunk = 10;                    /* set loop iteration chunk size */

	/*** Spawn a parallel region explicitly scoping all variables ***/
	#pragma omp parallel shared(matrix,nthreads,chunk) private(tid,i,j,k,x)
	{
		tid = omp_get_thread_num();
		if (tid == 0)
		{
			nthreads = omp_get_num_threads();
			printf("Starting matrix multiple example with %d threads\n",nthreads);
			printf("Initializing matrices...\n");
		}
		/*** Initialize matrices ***/
		#pragma omp for schedule (dynamic, chunk) 
		for (i=0; i < 10; i++)
			for (j=0; j < 10; j++)
				matrix[i][j]= start_value;

		/*** Do matrix multiply sharing iterations on outer loop ***/
		/*** Display who does which iterations for demonstration purposes ***/
		printf("Thread %d starting matrix multiply...\n",tid);
		#pragma omp for schedule (dynamic, chunk)
		for (x=1; x <= nb_iteration; x++)
		{
			for (i=0; i < 10; i++)    
			{
				printf("Thread=%d did row=%d\n",tid,i);   
				for (j=0; j < 10; j++)
				{
					spinWait(50);
					matrix[i][j] = matrix[i][j] + i + j;
				}
			}
		}
	}   /*** End of parallel region ***/

	/*** Print results ***/
	printf("******************************************************\n");
	printf("Result Matrix:\n");
	for (i=0; i < 10; i++)
	  {
	  for (j=0; j < 10; j++) 
		printf("%d   ", matrix[i][j]);
	  printf("\n"); 
	  }
	printf("******************************************************\n");
	gettimeofday (&tp, NULL); // Fin du chronometre
    timeEnd = (double) (tp.tv_sec) + (double) (tp.tv_usec) / 1e6;
    Texec = timeEnd - timeStart; //Temps d'execution en secondes
    printf("durée d'exécution : %lf\n", Texec);
	printf ("Done.\n");
	
    return 0; 
}