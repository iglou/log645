#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <omp.h>
#include "sys/time.h"

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
  //CHRONOMETRE SEQUENTIEL
  double timeStart1, timeEnd1, Texec1;
  //CHRONOMETRE PARALLELE
  double timeStart2, timeEnd2, Texec2;
  struct timeval tp;

  int i, j, chunk, x, matrix_par[10][10], matrix_seq[10][10], start_value, nb_iteration, probleme;

  //Initialisation des paramètres
  probleme = atoi(argv[1]);
  start_value = atoi(argv[2]);
  nb_iteration = atoi(argv[3]);
  chunk = 10;

  //Initialisation matrice en sequentiel
  for (i = 0; i < 10; i++) {
    for (j = 0; j < 10; j++)  matrix_seq[i][j] = start_value;
  }

  /***********************************PROBLEME1***************************************/
  if(probleme == 1){
    //SEQUENTIEL
    gettimeofday (&tp, NULL); //Chronomètre
    timeStart1 = (double) (tp.tv_sec) + (double) (tp.tv_usec) / 1e6;

    for (x = 1; x <= nb_iteration; x++)
    {
      for (i = 0; i < 10; i++) {
        for (j = 0; j < 10; j++) {
          spinWait(50);
          matrix_seq[i][j] = matrix_seq[i][j] + i + j;
        }
      }
    }

    gettimeofday (&tp, NULL); // Fin du chronometre
    timeEnd1 = (double) (tp.tv_sec) + (double) (tp.tv_usec) / 1e6;
    Texec1 = timeEnd1 - timeStart1;

    //PARELLELE
    gettimeofday (&tp, NULL); //Chronomètre
    timeStart2 = (double) (tp.tv_sec) + (double) (tp.tv_usec) / 1e6;

    #pragma omp parallel shared(matrix_par,chunk,x,i) private(j)
    {
      //Initialisation des matrices
      #pragma omp for schedule (dynamic, chunk) collapse(2)
      for (i=0; i < 10; i++)
        for (j=0; j < 10; j++)
          matrix_par[i][j]= start_value;

      //Calcul de la matrice
      #pragma omp for schedule(static,1) collapse(3)
      for (x=1; x <= nb_iteration; x++)
      {     
        for (i=0; i < 10; i++)
        {   
          for (j=0; j < 10; j++)
          {
            spinWait(50);
            matrix_par[i][j] = matrix_par[i][j] + i + j;
          }
        }
      }
    }
    gettimeofday (&tp, NULL); // Fin du chronometre
    timeEnd2 = (double) (tp.tv_sec) + (double) (tp.tv_usec) / 1e6;
    Texec2 = timeEnd2 - timeStart2;
  }

  /***********************************PROBLEME2***************************************/
  else if(probleme == 2){
    //SEQUENTIEL
    gettimeofday (&tp, NULL); // Debut du chronometre séquentiel
    timeStart1 = (double) (tp.tv_sec) + (double) (tp.tv_usec) / 1e6;
      
    for (x = 1; x <= nb_iteration; x++){
      for (i = 0; i < 10; i++) {
        for (j = 9; j >= 0; j--){
          spinWait(50);
          if (j == 9) matrix_seq[i][j] = matrix_seq[i][j] + i;
          else        matrix_seq[i][j] = matrix_seq[i][j] + matrix_seq[i][j + 1];
        }
      }
    }
    gettimeofday (&tp, NULL); // Fin du chronometre
    timeEnd1 = (double) (tp.tv_sec) + (double) (tp.tv_usec) / 1e6;
    Texec1 = timeEnd1 - timeStart1;

    //PARELLELE
    gettimeofday (&tp, NULL); //Chronomètre
    timeStart2 = (double) (tp.tv_sec) + (double) (tp.tv_usec) / 1e6;
    #pragma omp parallel shared(matrix_par,chunk,x,j) private(i)
    {
      //Initialisation des matrices
      #pragma omp for schedule (dynamic, chunk) collapse(2)
      for (i=0; i < 10; i++)
        for (j=0; j < 10; j++)
          matrix_par[i][j]= start_value;

      //Calcul de la matrice
      #pragma omp for schedule(dynamic,chunk) collapse(3)
      for (x=1; x <= nb_iteration; x++)
      {
        for (i=0; i < 10; i++)
        {
          for (j=9; j >= 0; j--)
          {
            spinWait(50);
            if(j == 9){
              matrix_par[i][j]= matrix_par[i][j] + i;
            }
            else{
              matrix_par[i][j] = matrix_par[i][j] + matrix_par[i][j+1];
            }
          }
        }
      }
    }
    gettimeofday (&tp, NULL); // Fin du chronometre
    timeEnd2 = (double) (tp.tv_sec) + (double) (tp.tv_usec) / 1e6;
    Texec2 = timeEnd2 - timeStart2;
  }

  //RESULTAT
  printf("******************************************************\n");
  printf("Resultat matrice en séquentiel:\n");
  for (i=0; i < 10; i++)
  {
    for (j=0; j < 10; j++) 
      printf("%d   ", matrix_seq[i][j]);
    printf("\n"); 
  }
  printf("******************************************************\n");
  printf("Resultat matrice en parallele:\n");
  for (i=0; i < 10; i++)
  {
    for (j=0; j < 10; j++) 
      printf("%d   ", matrix_par[i][j]);
    printf("\n"); 
  }
  printf("******************************************************\n");
  printf("durée d'exécution séquentiel: %lf\n", Texec1);
  printf("durée d'exécution parallele : %lf\n", Texec2);
  printf("******************************************************\n");
  
  return 0; 
}