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

//Struttura dati
typedef struct {
    int n;
    int **matrice;
    int min;
    int max;
    int inizio_riga;
    int fine_riga;
} threadData;

void *cercaMinMax(void *arg) {
    threadData *data = (threadData*)arg;
    int min = data->matrice[data->inizio_riga][0];
    int max = data->matrice[data->inizio_riga][0];

    for(int i = data->inizio_riga; i <= data->fine_riga; i++) {
        for(int j = 0; j < data->n; j++) {
            if(data->matrice[i][j] < min) {
                min = data->matrice[i][j];
            }
            if(data->matrice[i][j] > max) {
                max = data->matrice[i][j];
            }
        }
    }

    data->min = min;
    data->max = max;
    
    pthread_exit(NULL);
}

int main() {
    int n;

    printf("Inserisci dimensione matrice e numero dei thread: ");
    scanf(" %d", &n);

    //Alloco matrice
    int **matrice = (int**)malloc(n*sizeof(int*));
    for (int i = 0; i < n; i++) {
        matrice[i] = (int*)malloc(n*sizeof(int));
    }

    //Assegno valori casuali alla matrice
    srand(time(NULL));
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < n; j++) {
            matrice[i][j] = rand()%10+1;
        }
    }

    //Stampa della matrice
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < n; j++) {
            printf("%d\t", matrice[i][j]);
        }
        printf("\n");
    }

    //Creazione dei thread
    pthread_t threads[n];
    //Creo un array di 'n' strutture data da poi passare ai thread
    threadData data[n];
    //Calcolo quante righe deve gestire ciascun thread

    for(int i = 0; i < n; i++) {
        data[i].n = n;
        data[i].matrice = matrice;
        data[i].inizio_riga = i;
        data[i].fine_riga = i;

        if (pthread_create(&threads[i], NULL, cercaMinMax, &data[i]) != 0) {
            printf("Errore nella creazione del thread\n");
            exit(EXIT_FAILURE);
        }
    }

    for(int i = 0; i < n; i++) {
        if(pthread_join(threads[i], NULL) != 0) {
            printf("Errore nel join del thread\n");
            exit(EXIT_FAILURE);
        }
    }

    // Trova il minimo e il massimo per ogni riga e visualizzali in output
    for (int i = 0; i < n; i++) {
        printf("Riga %d - Minimo: %d, Massimo: %d\n", i, data[i].min, data[i].max);
    }

    //Si inizializzano le variabili minimo e massimo con i valori ottenuti dal primo thread, rappresentato da data[0].
    int minimoGlobale = data[0].min;
    int massimoGlobale = data[0].max;

    //Itero attraverso gli altri elementi dell'array data, partendo dall'indice 1, poiché il primo elemento è già stato considerato.
    for(int i = 1; i < n; i++) {
        if(data[i].min < minimoGlobale) {
            minimoGlobale = data[i].min;
        }
        if(data[i].max > massimoGlobale) {
            massimoGlobale = data[i].max;
        }
    }

    printf("Valore Minimo Globale: %d\n", minimoGlobale);
    printf("Valore Massimo Globale: %d\n", massimoGlobale);

    return 0;
    
}