/*
    Traccia dell'esercizio:
    Scrivi un programma in linguaggio C che utilizza i thread per eseguire operazioni su due matrici. 
    
    Il programma dovrebbe prendere in input due matrici quadrate di dimensione NxN, dove N è un numero intero definito dall'utente. 
    Successivamente, il programma dovrebbe creare due thread che eseguono le seguenti operazioni:

    1. Il primo thread dovrebbe calcolare la somma degli elementi delle due matrici e salvare il risultato in una terza matrice.
    2. Il secondo thread dovrebbe calcolare il prodotto delle due matrici e salvare il risultato in una quarta matrice.

    Infine, il programma dovrebbe aspettare che entrambi i thread abbiano completato i loro calcoli e quindi stampare i risultati ottenuti.
*/

#include "apue.h"
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

//Creazione struttura dati da passare ai thread
typedef struct {
    int N;
    int **mat1;
    int **mat2;
    int **matSomma;
    int **matProdotto;
} datiMatrici; 

void *calcoloSomma(void *arg) {
    datiMatrici *dati = (datiMatrici *)arg;

    for(int i = 0; i < dati->N; i++) {
        for(int j = 0; j < dati->N; j++) {
            dati->matSomma[i][j] = dati->mat1[i][j] + dati->mat2[i][j];
        }
    }

    return NULL;
}

//Il prodotto tra due matrici A e B viene calcolato moltiplicando ogni elemento della riga i della matrice A 
//con l'elemento corrispondente della colonna j della matrice B, e sommando i risultati. 
//Questo viene fatto per ogni elemento della matrice prodotto.
void *calcoloProdotto(void *arg) {
    datiMatrici *dati = (datiMatrici *)arg;

    for(int i = 0; i < dati->N; i++) {
        for(int j = 0; j < dati->N; j++) {
            dati->matProdotto[i][j] = 0;
            for(int k = 0; k < dati->N; k++) {
                dati->matProdotto[i][j] += dati->mat1[i][k] * dati->mat2[k][j];
            }
        }
    }

    return NULL;
}


int main() {
    int N;

    printf("Inserisci dimensione matrici **mat1 e **mat2 (NxN) -> ");
    scanf(" %d", &N);

    //Allocazione dinamica delle matrici
    int **mat1 = (int **)malloc(N*sizeof(int*));
    int **mat2 = (int **)malloc(N*sizeof(int*));
    int **matSomma = (int **)malloc(N*sizeof(int*));
    int **matProdotto = (int **)malloc(N*sizeof(int*));
    for (int i = 0; i < N; i++) {
        mat1[i] = (int *)malloc(N*sizeof(int));
        mat2[i] = (int *)malloc(N*sizeof(int));
        matSomma[i] = (int *)malloc(N*sizeof(int));
        matProdotto[i] = (int *)malloc(N*sizeof(int));
    }

    //Assegnazione casuale valori delle matrici
    srand(time(NULL));
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            mat1[i][j] = rand() % 10 + 1;
            mat2[i][j] = rand() % 10 + 1;
        }
    }

    //Creazione dei thread
    pthread_t threadSomma, threadProdotto;

    //Creazione della struttura dati da passare ai thread
    datiMatrici dati;
    dati.mat1 = mat1;
    dati.mat2 = mat2;
    dati.matSomma = matSomma;
    dati.matProdotto = matProdotto;
    dati.N = N;

    if ((pthread_create(&threadSomma, NULL, calcoloSomma, (void*)&dati)) != 0) {
        printf("Errore nella creazione del thread.");
        exit(-1);
    }

    if ((pthread_create(&threadProdotto, NULL, calcoloProdotto, (void*)&dati)) != 0) {
        printf("Errore nella creazione del thread.");
        exit(-1);
    }

    if ((pthread_join(threadSomma, NULL)) != 0) {
        printf("Errore nel join del thread.");
        exit(-1);
    }
    
    if ((pthread_join(threadProdotto, NULL)) != 0) {
        printf("Errore nel join del thread.");
        exit(-1);
    }

    //Stampa della matrice **mat1
    printf("**mat1 e **mat2 : \n");
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            printf("%d\t", mat1[i][j]);
        }
        printf("\n");
    }

    //Stampa della matrice **mat2
    printf("**mat2 : \n");
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            printf("%d\t", mat2[i][j]);
        }
        printf("\n");
    }

    //Stampa della matrice **matSomma
    printf("Somma delle matrici:\n");
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            printf("%d\t", matSomma[i][j]);
        }
        printf("\n");
    }

    //Stampa della matrice **matProdotto
    printf("\nProdotto delle matrici:\n");
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            printf("%d\t", matProdotto[i][j]);
        }
        printf("\n");
    }
    return 0;
}