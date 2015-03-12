#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <mpi.h>
#include "sys/time.h"

void mastercode (int nb_col,int nb_ligne, int np);
void slavecode (double td, double h);

int main (int argc, char *argv[])
{
    int nb_col, nb_ligne, np;
    double td, h;
    double timeStart, timeEnd, Texec;
    struct timeval tp;

    nb_col = 15;
    nb_ligne = 10;
    np = 50;
    td = 0.0002;
    h = 0.1;

    int rank, nprocs;
    MPI_Init (&argc, &argv);
    MPI_Comm_rank (MPI_COMM_WORLD, &rank);
    MPI_Comm_size (MPI_COMM_WORLD, &nprocs);

    printf("Entrée dans le prog principal par %d\n", rank);

    if (argc != 3){
        printf("Veuillez entrez le nombre correct de paramètres : 5\n");
        MPI_Finalize ();
        return 1;
    }
    // Debut du chronometre
    gettimeofday (&tp, NULL); 
    timeStart = (double) (tp.tv_sec) + (double) (tp.tv_usec) / 1e6;
    if (rank == 0){
        printf("Entree dans le master\n");
        mastercode (nb_col, nb_ligne, np);
    }
    else{
        printf("Entree dans le slave\n");
        slavecode (td, h);
    }
    gettimeofday (&tp, NULL); // Fin du chronometre
    timeEnd = (double) (tp.tv_sec) + (double) (tp.tv_usec) / 1e6;
    Texec = timeEnd - timeStart; //Temps d'execution en secondes
    printf("temps dexécution : %f\n", Texec);
    MPI_Finalize ();

    printf("FIN prg\n");
    return 0;
}

void mastercode (int nb_col,int nb_ligne, int np)
{
  int i=0, j=1, ii, jj, a=1, nprocs, rank=1, task[7], mat0=0, mat1=1;
  double answer[3];
  MPI_Status status;
  int answers_to_receive, received_answers;
  MPI_Comm_size (MPI_COMM_WORLD, &nprocs);

   /*Initialisation de la matrice*/
    double matrix[nb_ligne][nb_col][2];
    for (i = 0; i < nb_ligne; i++) {
        for (j = 0; j < nb_col; j++){
            matrix[i][j][0] = i*(nb_ligne-i-1)*j*(nb_col-j-1);
            matrix[i][j][1] = i*(nb_ligne-i-1)*j*(nb_col-j-1);
        }
    }

    answers_to_receive = nb_ligne*nb_col-2*nb_ligne-2*nb_col+4;
    received_answers = 0;

    /* send tasks to slaves */
    i=1;
    j=1;
    while(i<nb_ligne-1){
      while(j<nb_col-1){
        task[0] = matrix[i][j][0];
        task[1] = matrix[i+1][j][0];
        task[2] = matrix[i-1][j][0];
        task[3] = matrix[i][j+1][0];
        task[4] = matrix[i][j-1][0];
        task[5] = i;
        task[6] = j;
        MPI_Send (&task[0], 7, MPI_DOUBLE, rank%(nprocs-1)+1, 1, MPI_COMM_WORLD);
        //printf("Envoi1 cellule %d %d à rank %d\n",i, j, rank%(nprocs-1)+1);
        if(rank == nprocs) break;
        j++;
        rank++;
      }
      if(rank == nprocs) break;
      i++;
      j = 1;
    }
    //get next cellule
    if(j == nb_col-2){
      i++;
      j = 1;
    }
    else{
      j++;
    }

    while(a <= np){
      while (received_answers < answers_to_receive){
          /* wait for an answer from a slave. */
        MPI_Recv (&answer[0], 3, MPI_DOUBLE, MPI_ANY_SOURCE, 1, MPI_COMM_WORLD, &status);
        //printf("Master recoit de slave %d valeur %lld pour cellule %lld %lld\n", status.MPI_SOURCE, answer[0], answer[1], answer[2]);
        ii = (int)(answer[1]);
        jj = (int)(answer[2]);
        matrix[ii][jj][mat1] = answer[0];
        received_answers++;       /* and the number of received answers */

        task[0] = matrix[i][j][mat0];
        task[1] = matrix[i+1][j][mat0];
        task[2] = matrix[i-1][j][mat0];
        task[3] = matrix[i][j+1][mat0];
        task[4] = matrix[i][j-1][mat0];
        task[5] = i;
        task[6] = j;
        MPI_Send (&task[0], 7, MPI_DOUBLE, status.MPI_SOURCE, 1, MPI_COMM_WORLD);
        //printf("Envoi2 cellule %d %d à slave %d\n",i, j, rank%(nprocs-1)+1);
        if(j == nb_col-1){
          i++;
          j = 1;
        }
        else{
          j++;
        }
      }
      printf("Matrice obtenue pour np = %d: \n", a);
        for (i = 0; i < nb_ligne; i++) {
            for (j = 0; j < nb_col; j++)     printf("%.2f    ", matrix[i][j][mat1]);
            printf("\n");
        }
      received_answers = 0;
      i = 1;
      j = 1;
      a++;
      if(mat0 == 0) mat0 = 1;
      else          mat0 = 0;
      if(mat1 == 0) mat1 = 1;
      else          mat1 = 0;
    }

    //Affichage de la matrice de fin
    if(np%2 == 0){
        printf("Matrice obtenue 1: \n");
        for (i = 0; i < nb_ligne; i++) {
            for (j = 0; j < nb_col; j++)     printf("%.2f    ", matrix[i][j][1]);
                printf("\n");
        }
    }
    else{
        printf("Matrice obtenue 0: \n");
        for (i = 0; i < nb_ligne; i++) {
            for (j = 0; j < nb_col; j++)     printf("%.2f    ", matrix[i][j][0]);
                printf("\n");
        }
    }
    printf("FIN MASTER\n");

//Now master sends a message to the slaves to signify that they should end the calculations. We use a special tag for that:

  /*counts = (int*) malloc(sizeof(int)*(nprocs-1));
  for (who = 1; who < nprocs; who++)
    {
      MPI_Send (&task[0], 1, MPI_INT, who, 2, MPI_COMM_WORLD);
      MPI_Recv (&counts[who-1], 1, MPI_INT, who, 7, MPI_COMM_WORLD, &status);
    }

  for (i = 1; i < nprocs; i++)
    {
      printf ("%6d %8d\n", i, counts[i-1]);
    }*/
}

void slavecode (double td, double h)
{
  int rank;
  double answer[3];
  int task[5];
  MPI_Status status;

  MPI_Comm_rank(MPI_COMM_WORLD,&rank);
  /* slave first enters 'waiting for message' state*/
  while(1){
    MPI_Recv (&task[0], 7, MPI_DOUBLE, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
    //printf("Slave %d recoit %d pour cellule %d %d\n", rank, task[0], task[5], task[6]);
    usleep(5);
    answer[0] = ((1-(4*td)/(h*h))*task[0])+((td/(h*h))*(task[1]+task[2]+task[3]+task[4]));    
    answer[1] = task[5];
    answer[2] = task[6];
    MPI_Send (&answer[0], 3, MPI_DOUBLE, 0, 1, MPI_COMM_WORLD);
    //printf("Slave %d envoie %lld pour cellule %lld %lld\n", rank, answer[0], answer[1], answer[2]);
  }
}