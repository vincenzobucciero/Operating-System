/**
    Scrivere un programma C in ambiente Linux che facendo uso della libreria Pthread e dei semafori POSIX realizzi il seguente comportamento:
   
    Ci sono due thread produttori e due thread consumatori. 
    
    C'è una risorsa condivisa (un buffer) di dimensione N.
    
    I produttori scrivono valori interi casuali nel buffer e i consumatori leggono i valori dal buffer.
    
    Il programma deve garantire che:
        - I produttori non scrivano nel buffer quando è pieno.
        - I consumatori non leggano dal buffer quando è vuoto.
        - Non ci siano conflitti nell'accesso al buffer tra i produttori e i consumatori.
    
    Utilizzare semafori POSIX per garantire la sincronizzazione tra i thread.
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define N 10

int numProcessiProd = 2;
int numProcessiCons = 2;
int buffer[N];

sem_t mutex;    //semaforo per l'accesso al buffer condiviso
sem_t empty;    //semaforo che tiene traccia dei posti vuoti nel buffer
sem_t full;     //semaforo che tiene traccia dei posti pieni nel buffer

void *threadProduttore(void *arg) {
    while(1){
        sem_wait(&empty);
        sem_wait(&mutex);
        int val = rand() % 100;
        printf("Prod: %d\n", val);
        buffer[rand() % N] = val;
        sem_post(&mutex);
        sem_post(&full);
        sleep(1);
    }
    pthread_exit(NULL);
}

void *threadConsumatore(void *arg) {
    while(1){
        sem_wait(&full);
        sem_wait(&mutex);
        int val = buffer[rand() % N];
        printf("Cons: %d\n", val);
        buffer[rand() % N] = 0;
        sem_post(&mutex);
        sem_post(&empty);
        sleep(1);
    }
    pthread_exit(NULL);
}

int main() {

    pthread_t threadProd[numProcessiProd];
    pthread_t threadCons[numProcessiCons];

    sem_init(&mutex, 0, 1);
    sem_init(&empty, 0, N);
    sem_init(&full, 0, 0);

    // Crea i thread produttori
    for (int i = 0; i < numProcessiProd; i++) {
        pthread_create(&threadProd[i], NULL, threadProduttore, &i);
    }

    // Crea i thread consumatori
    for (int i = 0; i < numProcessiCons; i++) {
        pthread_create(&threadCons[i], NULL, threadConsumatore, &i);
    }

    // Attende la terminazione dei thread produttori
    for (int i = 0; i < numProcessiProd; i++) {
        pthread_join(threadProd[i], NULL);
    }

    // Attende la terminazione dei thread consumatori
    for (int i = 0; i < numProcessiCons; i++) {
        pthread_join(threadCons[i], NULL);
    }

    sem_destroy(&mutex);
    sem_destroy(&empty);
    sem_destroy(&full);

    return 0;
}