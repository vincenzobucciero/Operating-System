
/**
 * -> Scrivere un programma che accetta un intero n di
 *      riga di comando, crea n thread e poi aspetta la loro 
 *      terminazione.
 * -> Ciascun thread aspetta un numero di secondi casuale
 *      tra 1 e 10, poi incrementa una variabile globale intera
 *      ed infine ne stampa il valore.
*/

#include "apue.h"
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

int var_glob = 0;

void *routine() {
    int n = rand() % 10 + 1;
    printf("Thread %ld sleeping for %d seconds\n", pthread_self(), n);
    sleep(n);
    var_glob++;
}

int main() {
    int n, rc;
    pthread_t tid[n];
    int *taskids[n];

    printf("Inserisci numero di thread:  ");
    scanf(" %d", &n);

    for(int i=0; i<n; i++) {
        taskids[i] = (int *) malloc(sizeof(int));
        *taskids[i] = i;
        printf("Creating thread %d\n", i);
        rc = pthread_create(&tid[i], NULL, routine, (void *) taskids[i]);

        if (rc)
        {
            printf("ERROR, return code from pthread_create() is %d\n", rc);
            exit(-1);
        }
        
    }
    printf("Variabile globale:  %d\n", var_glob);

    pthread_exit(NULL);

}


