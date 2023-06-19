/**
 * Realizzare un programma C e Posix sotto Linux che, mediante l'ausilio della libreria pthread, 
 * decomponga la  somma  degli  elementi  di  un  vettore  di  interi  di  dimensione  1000  in  quattro  somme  locali  
 * effettuate  concorrentemente da altrettanti thread. 
 * Si contempli l'uso di una variabile mutex per regolare l'accesso alla variabile globale che conterrà la somma degli elementi del vettore. 
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define ARRAY_SIZE 1000
#define NUM_THREADS 4 // 1000 / 4 -> ogni thread getisce 250 elementi

int array[ARRAY_SIZE];
int sommaGlobale = 0;
pthread_mutex_t myMutex = PTHREAD_MUTEX_INITIALIZER;

void *routine(void *thread_id) {
    int tid = *(int *)thread_id;
    int inizioVettore = tid * (ARRAY_SIZE / NUM_THREADS);
    int fineVettore = inizioVettore + (ARRAY_SIZE / NUM_THREADS);
    int sommaLocale = 0;

    for(int i = inizioVettore; i < fineVettore; i++) {
        sommaLocale += array[i];
    }

    pthread_mutex_lock(&myMutex);
    sommaGlobale += sommaLocale;
    pthread_mutex_unlock(&myMutex);

    pthread_exit(NULL);
}

int main() {
    pthread_t threads[NUM_THREADS];
    int threadIds[NUM_THREADS];

    srand(time(NULL));
    for(int i = 0; i < ARRAY_SIZE; i++) {
        array[i] = rand()%10+1;
    }

    for (int i = 0; i < ARRAY_SIZE; i++) {
        printf("%d  ", array[i]);
    }
    

    // Crea i thread
    for (int i = 0; i < NUM_THREADS; i++) {
        threadIds[i] = i;
        pthread_create(&threads[i], NULL, routine, (void*)&threadIds[i]);
    }

    // Aspetta la terminazione dei thread
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    printf("Somma globale: %d\n", sommaGlobale);
}