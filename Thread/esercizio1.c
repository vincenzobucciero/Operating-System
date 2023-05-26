
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
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void *routine() {
    int n = rand() % 2 + 1;
    printf("Thread %ld sleeping for %d seconds\n", pthread_self(), n);
    sleep(n);

    pthread_mutex_lock(&mutex);
    var_glob++;
    pthread_mutex_unlock(&mutex);

    pthread_exit(NULL);
}

int main() {
    int n, rc;

    printf("Inserisci numero di thread:  ");
    scanf(" %d", &n);

    pthread_t *tid = malloc(n*sizeof(pthread_t));
    // int **taskids = malloc(n*sizeof(int*));

    for(int i = 0; i < n; i++) {
        //taskids[i] = malloc(sizeof(int));
        //*taskids[i] = i;
        printf("Creating thread %d\n", i);
        rc = pthread_create(&tid[i], NULL, routine, NULL);

        if (rc)
        {
            printf("ERROR, return code from pthread_create() is %d\n", rc);
            exit(-1);
        }  
    }

    for (int i = 0; i < n; i++) {
        rc = pthread_join(tid[i], NULL);
        if (rc) {
            printf("ERROR, return code from pthread_join() is %d\n", rc);
            exit(-1);
        }
    }

    printf("Variabile globale:  %d\n", var_glob);

    return 0;

}


