
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

//variabile globale da incrementare
int var_glob = 0;

void *routine(void *arg) {
    //Variabile per l'identificazione del thread
    int threadId = *((int*)arg);

    //Genera un numero casuale tra 1 e 2 per la sleep
    int n = rand() % 10 + 1;
    
    printf("Thread %d -> var glob %d\n", threadId, threadId);
    printf("Thread %d sleep for %d second(s)\n", threadId, n);

    sleep(n);

    //Incremento la variabile
    var_glob++;

    //Termina il thread
    pthread_exit(NULL);
}

int main() {
    /*
    int n, rc;

    //Legge il numero di thread da creare dall'input
    printf("Inserisci numero di thread:  ");
    scanf(" %d", &n);

    //Alloca spazio per gli ID dei thread
    pthread_t *tid = malloc(n*sizeof(pthread_t));

    for(int i = 0; i < n; i++) {
        //Crea thread
        printf("Creating thread %d\n", i);
        rc = pthread_create(&tid[i], NULL, routine, NULL);

        if (rc)
        {
            printf("ERROR, return code from pthread_create() is %d\n", rc);
            exit(-1);
        }  
    }

    for (int i = 0; i < n; i++) {
        //Aspetta la terminazione di ciascun thread
        rc = pthread_join(tid[i], NULL);

        if (rc) {
            printf("ERROR, return code from pthread_join() is %d\n", rc);
            exit(-1);
        }
    }

    //Stampa il valore finale della variabile globale
    printf("Variabile globale:  %d\n", var_glob);

    return 0;
    */

    int n;

    //Legge il numero di thread da creare dall'input
    printf("Inserisci numero di thread:  ");
    scanf(" %d", &n);

    pthread_t* threads = (pthread_t*)malloc(n * sizeof(pthread_t));

    int* threadIds = (int*)malloc(n * sizeof(int));

    // Crea n thread
    for (int i = 0; i < n; i++) {
        threadIds[i] = i + 1;
        pthread_create(&threads[i], NULL, routine, &threadIds[i]);
    }

    // Attendi la terminazione dei thread
    for (int i = 0; i < n; i++) {
        pthread_join(threads[i], NULL);
    }

    printf("Variabile globale totale: %d\n", var_glob);

    free(threads);
    free(threadIds);

    return 0;

}


