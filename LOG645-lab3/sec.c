#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "sys/time.h"

int main (int argc, char *argv[]){  
    
    int nb_col, nb_ligne, x, i, j, np;
    double td, h;
    double timeStart, timeEnd, Texec;
    struct timeval tp;

    //Initialisation des paramètres
    nb_col = atoi(argv[1]);
    nb_ligne = atoi(argv[2]);
    np = atoi(argv[3]);
    td = atof(argv[4]);
    h = atof(argv[5]);

    printf("nombre colonnes : %d\n", nb_col);
    printf("nombre lignes : %d\n", nb_ligne);
    printf("nombre d'iteration : %d\n", np);
    
    // Debut du chronometre
    gettimeofday (&tp, NULL); 
    timeStart = (double) (tp.tv_sec) + (double) (tp.tv_usec) / 1e6;
    //Initialisation matrice
    double matrix[nb_ligne][nb_col][2];
    for (i = 0; i < nb_ligne; i++) {
        for (j = 0; j < nb_col; j++){
            matrix[i][j][0] = i*(nb_ligne-i-1)*j*(nb_col-j-1);
        }
    }
    //Affichage de la matrice de départ
    printf("Matrice initiale : \n");
    for (i = 0; i < nb_ligne; i++) {
        for (j = 0; j < nb_col; j++)     printf("%.2f    ", matrix[i][j][0]);
            printf("\n");
    }

    for(x = 1; x < np; x++){
        for(i = 1; i < nb_ligne-1; i++){
            for (j = 1; j < nb_col-1; j++){
                usleep(50);
                if(x%2 == 0){
                    matrix[i][j][0]=((1-(4*td)/(h*h))*matrix[i][j][1])+((td/(h*h))*(matrix[i-1][j][1]+matrix[i+1][j][1]+matrix[i][j-1][1]+matrix[i][j+1][1]));
                }
                else{
                    matrix[i][j][1]=((1-(4*td)/(h*h))*matrix[i][j][0])+((td/(h*h))*(matrix[i-1][j][0]+matrix[i+1][j][0]+matrix[i][j-1][0]+matrix[i][j+1][0]));
                }
            }
        }
    }
    gettimeofday (&tp, NULL); // Fin du chronometre
    timeEnd = (double) (tp.tv_sec) + (double) (tp.tv_usec) / 1e6;
    Texec = timeEnd - timeStart; //Temps d'execution en secondes

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

    //Affichage du temps d'exécution
    printf("temps dexécution : %f\n", Texec);

    return 0;
}