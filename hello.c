#include <stdio.h>

void main (int argc, char *argv[]){
	printf("Hello world!\n" );
	calcul();
	int matrix[8][8];
	int i;
	int j;
	for(i=0; i<8; i++){
		for(j=0; j<8; j++){
			matrix[i][j]=3;
		}
	}

	int indice = 2;

	int matrix2[8][8];
	for(i=0; i<8; i++){
		for(j=0; j<8; j++){
			matrix2[i][j]=matrix[i][j]+(i+j);
			printf("%d", matrix2[i][j]);
			printf("\t");
		}
		printf("\n");
	}


	return(0);
}

void calcul (){
	int index;
	printf("Entre un indice : \n");
	scanf("%d", &index);
	printf("Indice choisi : %d\n", index);

	return;
}