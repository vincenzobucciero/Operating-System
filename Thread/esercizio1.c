
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
//mutex per l'accesso sincronizzato alla variabile globare condivisa
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void *routine() {
    // Genera un numero casuale tra 1 e 2 per la sleep
    int n = rand() % 2 + 1;
    printf("Thread %ld sleeping for %d seconds\n", pthread_self(), n);
    sleep(n);

    //Acquisisce il lock del mutex per proteggere l'accesso alla variabile globale
    pthread_mutex_lock(&mutex);
    //Incremento la variabile
    var_glob++;
    //Sblocco il lock del mutex
    pthread_mutex_unlock(&mutex);

    //Termina il thread
    pthread_exit(NULL);
}

int main() {
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

}


