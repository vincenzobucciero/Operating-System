/*
    Si realizzi un programma in C e Posix sotto Linux che, con l’ausilio della libreria Pthread, 
    lancia n thread per cercare il valore minimo ed il valore massimo in una matrice nxn di interi. 
    La ricerca del minimo e del massimo deve essere effettuata simultaneamente su tutte le righe. 
    Individuati i valori cercati, si provvederà a stamparli a video. 
    La dimensione della matrice può essere fornita in input al programma in fase di esecuzione o da riga di comando. 
*/

#include "apue.h"
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>



int main() {
    int n;

    printf("Inserisci dimensione matrice : ");
    scanf(" %d", &n);

    int **matrice = (int**)malloc(n*sizeof(int*));
    for (int i = 0; i < n; i++) {
        matrice[i] = (int*)malloc(n*sizeof(int));
    }

    srand(time(NULL));
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < n; j++) {
            matrice[i][j] = rand()%10+1;
        }
    }

    for(int i = 0; i < n; i++) {
        for(int j = 0; j < n; j++) {
            printf("%d\t", matrice[i][j]);
        }
        printf("\n");
    }

    return 0;
    
}