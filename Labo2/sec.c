/******************************************************************************
* FILE: omp_hello.c
* DESCRIPTION:
*   OpenMP Example - Hello World - C/C++ Version
*   In this simple example, the master thread forks a parallel region.
*   All threads in the team obtain their unique thread number and print it.
*   The master thread only prints the total number of threads.  Two OpenMP
*   library routines are used to obtain the number of threads and each
*   thread's number.
* AUTHOR: Blaise Barney  5/99
* LAST REVISED: 04/06/05
******************************************************************************/
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>

void spinWait(int milliseconds)
{
  struct timeval startTime;
  struct timeval endTime;
  gettimeofday(&startTime, NULL);
  do{
    gettimeofday(&endTime, NULL);
  } while ((endTime.tv_sec - startTime.tv_sec) * 1000000 + (endTime.tv_usec - startTime.tv_usec)< milliseconds * 1000);
  return;
}

int main (int argc, char *argv[]) {
  int nthreads, tid;
  int myid, numprocs, tag, probleme, start_value, nb_iteration, x, i,j, maxtrix[10][10];
  double timeStart, timeEnd, Texec;
  struct timeval tp;

  //Initialisation des paramètres
    probleme = atoi(argv[1]);
    start_value = atoi(argv[2]);
    nb_iteration = atoi(argv[3]);
  
  //Initialisation de la matrice
  for (i = 0; i < 10; i++) {
    for (j = 0; j < 10; j++)  maxtrix[i][j] = start_value;
  }
  
  if(nb_iteration != 0){
    if(probleme == 1)
    {
      gettimeofday (&tp, NULL); // Debut du chronometre
      timeStart = (double) (tp.tv_sec) + (double) (tp.tv_usec) / 1e6;

      for (x = 1; x <= nb_iteration; x++)
      {
        for (i = 0; i < 10; i++) {
          for (j = 0; j < 10; j++) {
            spinWait(50);
            maxtrix[i][j] = maxtrix[i][j] + i + j;
          }
        }
      }
    }
    else if(probleme == 2){
      gettimeofday (&tp, NULL); // Debut du chronometre
      timeStart = (double) (tp.tv_sec) + (double) (tp.tv_usec) / 1e6;
      
      for (x = 1; x <= nb_iteration; x++){
        for (i = 0; i < 10; i++) {
          for (j = 9; j >= 0; j--){
            spinWait(50);
            if (j == 9) maxtrix[i][j] = maxtrix[i][j] + i;
            else        maxtrix[i][j] = maxtrix[i][j] + maxtrix[i][j + 1];
          }
        }
      }
    }
    else{
      printf("Numéro de problème inexistant\n");
    }
  }

  //Affichage de la matrice de fin
  printf("Matrice obtenue : \n");
  for (i = 0; i < 10; i++) {
    for (j = 0; j < 10; j++)   printf("%d    ",maxtrix[i][j]);
    printf("\n");
  }

  gettimeofday (&tp, NULL); // Fin du chronometre
  timeEnd = (double) (tp.tv_sec) + (double) (tp.tv_usec) / 1e6;
  Texec = timeEnd - timeStart; //Temps d'execution en secondes

  printf("durée d'exécution : %lf\n", Texec);
  return 0;

}

