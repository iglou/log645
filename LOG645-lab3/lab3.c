#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <mpi.h>
#include "sys/time.h"

void mastercode (int n,int m);
void slavecode (void);

int main (int argc, char *argv[])
{
    int nb_col, nb_ligne, x, np;
    double td, h;

    nb_col = 15;
    nb_ligne = 10;
    np = 50;
    td = 0.0002;
    h = 0.1;

    int rank, nprocs;
    MPI_Init (&argc, &argv);
    MPI_Comm_rank (MPI_COMM_WORLD, &rank);
    MPI_Comm_size (MPI_COMM_WORLD, &nprocs);

    if (argc != 3){
        printf("Veuillez entrez le nombre correct de paramètres : 5\n");
        MPI_Finalize ();
        return 1;
    }

    if (rank == 0){
        mastercode (nb_col, nb_ligne, np);
    }
    else{
        slavecode (td, h);
    }
    MPI_Finalize ();
    return 0;
}

void mastercode (int nb_col,int nb_ligne, int np)
{
  int i, j;
  long long answer;
  int who, nprocs, rank=1;
  int task[2];
  MPI_Status status;

  long long sum = 0;

  int answers_to_receive, received_answers;
  int *counts;
  int whomax,num;

  MPI_Comm_size (MPI_COMM_WORLD, &nprocs);


  /*
   * This code is somewhat complicated. The objective is that master keeps on receiving answers and sending tasks from and
   * to slaves. In order to end the program properly, we need to know in which state the slaves are. They can be in two states:
   * - sending a message to master
   * - waiting for a message from master
   * We want to make sure, that at the end of the program all 
   * slaves are waiting for a message from master, so master can ask them to stop.
   *
   * It is also important that master sends exactly the right amount
   * of tasks to the slaves, and receives exactly the right amount of answers.
   *
   * There are several ways to accomplish this. In this example we present the most simple method.
   *
   * The slaves start in the state 'waiting for a message'.
   * Master sends each of the slaves a task. Care has to be taken for the case that there a less tasks than slaves.
   *
   * When a slave is in the state 'waiting for message', it accepts two kinds of tasks: 
   * 1 perform calculations, send result back to master and enter the 'waiting' state again
   * 2 send the number of tasks performed to master and stop
   *
   * After sending the first task to the slaves, master enters a loop, receiving answers and sending
   * new tasks, until the desired number of answers is received and taking care that not too many tasks are dispatched.
   * Then master sends the second task ('stop' signal) to the slaves whereupon the whole machinery stops.
   */

   /*Initialisation de la matrice*/
    double matrix[nb_ligne][nb_col][2];
    for (i = 0; i < nb_ligne; i++) {
        for (j = 0; j < nb_col; j++){
            matrix[i][j][0] = i*(nb_ligne-i-1)*j*(nb_col-j-1);
        }
    }

    answers_to_receive = nb_ligne*nb_col;
    //(n-1)/m + 1;
    received_answers = 0;
    num = 1;

    /* send tasks to slaves */
    whomax = nprocs-1;
    if (whomax > answers_to_receive) 
        whomax = answers_to_receive;

    for (i = 0; i < nb_ligne; i++) {
        for (j = 0; j < nb_col; j++){
            task[0] = matrix[i][j][0];
            task[1] = matrix[i+1][j][0];
            task[2] = matrix[i-1][j][0];
            task[3] = matrix[i][j+1][0];
            task[4] = matrix[i][j-1][0];
            MPI_Send (&task[0], 5, MPI_INT, /* sending two ints */
              rank%(nprocs-1)+1, 1,            /* tag */
              MPI_COMM_WORLD);      /* communicator */
            rank++;
        }
    }

    while (received_answers < answers_to_receive)
    {
        /* wait for an answer from a slave. */

        MPI_Recv (&answer,    /* address of receive buffer */
            1,      /* number of items to receive */
            MPI_LONG_LONG,  /* type of data */
            MPI_ANY_SOURCE, /* can receive from any other */
            1,      /* tag */
            MPI_COMM_WORLD, /* communicator */
            &status);   /* status  */

        who = status.MPI_SOURCE; /* find out who sent us the answer */
        
        sum += answer;        /* update the sum */
        received_answers++;       /* and the number of received answers */

      /* put the slave on work, but only if not all tasks have been sent. We use the value of num to detect this */

      if (num <= n)
      {
        task[0] = num;
        task[1] = num + m - 1;
        if (task[1] > n)
          task[1] = n;

        MPI_Send (&task[0], 4, MPI_INT, /* sending two ints */
              who,                      /* to the lucky one */
              1,                        /* tag */
              MPI_COMM_WORLD);          /* communicator */
        num += m;
      }
    }

//Now master sends a message to the slaves to signify that they should end the calculations. We use a special tag for that:

  counts = (int*) malloc(sizeof(int)*(nprocs-1));
  for (who = 1; who < nprocs; who++)
    {
      MPI_Send (&task[0], 1, MPI_INT,   /* sending one int 
                     * it is permitted to send a shorter message
                     * than will be received. The other case:
                     * sending a longer message than the receiver
                     * expects is not allowed. */
        who,            /* to who */
        2,          /* tag */
        MPI_COMM_WORLD);    /* communicator */

      /* the slave will send to master the number of calculations
       * that have been performed. We put this number in the counts array.*/

      MPI_Recv (&counts[who-1], /* address of receive buffer */
        1,      /* number of items to receive */
        MPI_INT,    /* type of data */
        who,        /* receive from process who */
        7,      /* tag */
        MPI_COMM_WORLD, /* communicator */
        &status);   /* status  */
    }

  printf ("The sum of the integers from 1..%d is %lld\n", n, sum);
  printf ("The work was divided using %d summations per slave\n",m);

  printf ("  SLAVE  calculations\n\n");
  for (i = 1; i < nprocs; i++)
    {
      printf ("%6d %8d\n", i, counts[i-1]);
    }
}

void slavecode (double td, double h)
{
  int i, rank;
  long long answer;
  int task[2];
  MPI_Status status;
  int count = 0;

  MPI_Comm_rank(MPI_COMM_WORLD,&rank);

  /* slave first enters 'waiting for message' state*/
  MPI_Recv (&task[0],       /* address of receive buffer */
        2,          /* number of items to receive */
        MPI_INT,        /* type of data */
        0,          /* can receive from master only */
        MPI_ANY_TAG,    /* can expect two values, so
                     * we use the wildcard MPI_ANY_TAG 
                     * here */
        MPI_COMM_WORLD, /* communicator */
        &status);       /* status  */

  /* if tag equals 2, then skip the calculations */

  if (status.MPI_TAG !=2)
  {
  while (1)
    {
        answer=((1-(4*td)/(h*h))*task[0])+((td/(h*h))*(task[1]+task[2]+task[3]+task[4]));
        MPI_Send (&answer, 1, /* sending one int  */
            MPI_LONG_LONG, 0,   /* to master */
            1,      /* tag */
            MPI_COMM_WORLD);    /* communicator */

        MPI_Recv (&task[0],   /* address of receive buffer */
            4,      /* number of items to receive */
            MPI_INT,    /* type of data */
            0,      /* can receive from master only */
            MPI_ANY_TAG,    /* can expect two values, so
                     * we use the wildcard MPI_ANY_TAG 
                     * here */
            MPI_COMM_WORLD, /* communicator */
            &status);   /* status  */

      if (status.MPI_TAG == 2)      /* leave this loop if tag equals 2 */
    break;
    }
  }
  
  /* this is the point that is reached when a task is received with tag = 2 */
  /* send the number of calculations to master and return */

  MPI_Send (&count, 1, MPI_INT, /* sending one int  */
        0,          /* to master */
        7,          /* tag */
        MPI_COMM_WORLD);    /* communicator */
}