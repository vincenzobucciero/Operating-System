/**
 * Si progetti un programma in ambiente Linux che, impiegando  la  libreria  pthread,  
 * crei un thread che estrae N volte (con N passato da riga di comando) 
 * un numero casuale compreso tra 0 e 10 e ne calcoli il fattoriale ponendolo in una variabile condivisa
 * e un thread che stampi a video il fattoriale calcolato N volte prelevandolo dalla variabile condivisa.
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

int n;
int variabileCondivisa;
pthread_mutex_t myMutex = PTHREAD_MUTEX_INITIALIZER;

void *routineEstrai(void *arg){
    srand(time(NULL));
    for(int i = 0; i < n; i++){
        int numeroCasuale = rand() % 10;
        int fattoriale = 1;

        // Calcola il fattoriale del numero estratto
        for(int j = 1; j <= numeroCasuale; j++){
            fattoriale *= j;
        }

        pthread_mutex_lock(&myMutex);
        variabileCondivisa = fattoriale;
        pthread_mutex_unlock(&myMutex);
    }
    pthread_exit(NULL);
}

void *routineStampa(void *arg){
    for(int i = 0; i < n; i++){
        pthread_mutex_lock(&myMutex); // Acquisisce il mutex prima di accedere alla variabile condivisa
        printf("Fattoriale: %d\n", variabileCondivisa);
        pthread_mutex_unlock(&myMutex); // Rilascia il mutex
    }

    pthread_exit(NULL);
}

int main(int argc, char *argv[]){
    if(argc < 2){
        perror("Inserisci input in compilazione\n");
        exit(EXIT_FAILURE);
    }

    n = atoi(argv[1]);

    pthread_t threads1, threads2;

    if(pthread_create(&threads1, NULL, routineEstrai, NULL) != 0){
        perror("Errore creazione thread estrai\n");
        exit(EXIT_FAILURE);
    }

    if(pthread_create(&threads2, NULL, routineStampa, NULL) != 0){
        perror("Errore creazione thread stampa\n");
        exit(EXIT_FAILURE);
    }

    if(pthread_join(threads1, NULL) != 0){
        perror("Errore join thread estrai\n");
        exit(EXIT_FAILURE);
    }

    if(pthread_join(threads2, NULL) != 0){
        perror("Errore join thread stampa\n");
        exit(EXIT_FAILURE);
    }

    return 0;
}