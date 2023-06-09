/**
 *  Facendo uso della libreria Pthread, realizzare un programma in cui un
 *  thread scrittore, dato un intero N da riga di comando (dove
 *  10<N<=15), scrive in un file nella prima posizione, uno alla volta ed
 *  ogni ½ secondo, la sequenza di Fibonacci di ordine N, alternandosi con
 *  un thread lettore che legge, uno alla volta dalla prima posizione del
 *  file, i numeri scritti dal thread scrittore. Un terzo thread attende la
 *  lettura dell’ N-esimo intero, quindi stampa a video il messaggio
 *  “Operazioni concluse, arrivederci dal thread: tid”, attende 5 secondi e
 *  termina.
*/


//SOLUZIONE CON CICLO FOR, QUINDI NON WHILE(1).

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>

#define FILE_NAME "fileGiusal1.txt"

typedef struct {
    int currentNumber;
    int totalNumber;
    sem_t semProd;
    sem_t semCons;
} sharedData;

int n;

void *routineProduttore(void *arg){
    sharedData *data = (sharedData *)arg;
    FILE *file = fopen(FILE_NAME, "w");

    if(file == NULL){
        perror("Errore apertura file\n");
        exit(EXIT_FAILURE);
    }

    int fib;
    int fib1 = 0;
    int fib2 = 1;

    fprintf(file, " %d\n", fib1);

    for(int i = 0; i < data->totalNumber; i++){
        sem_wait(&data->semProd);
        sleep(1/2);
        fib = fib1 + fib2;
        fprintf(file, " %d\n", fib);
        fflush(file);
        fib1 = fib2;
        fib2 = fib;
        sem_post(&data->semCons);
    }

    fclose(file);

    pthread_exit(NULL);
}

void *routineConsumatore(void *arg){
    sharedData *data = (sharedData *)arg;

    FILE *file = fopen(FILE_NAME, "r");

    if(file == NULL){
        perror("Errore apertura file\n");
        exit(EXIT_FAILURE);
    }

    for(int i = 0; i < data->totalNumber; i++){
        sem_wait(&data->semCons);
        fscanf(file, "%d", &data->currentNumber);
        printf("Numero fibo letto:  %d\n", data->currentNumber);
        sem_post(&data->semProd);
    }

    fclose(file);

    pthread_exit(NULL);
}

int main(int argc, char *argv[]){
    if(argc < 2){
        printf("Inserire un numero intero 10<=N<=15\n");
    }

    n = atoi(argv[1]);

    if (n < 10 || n > 15) {
        printf("Numero non valido. Inserire un numero intero 10<=N<=15\n");
        return 1;
    }

    sharedData data;
    data.currentNumber = 0;
    data.totalNumber = n;

    sem_init(&data.semProd, 0, 1);
    sem_init(&data.semCons, 0, 0);

    pthread_t produttore, consumatore;

    if(pthread_create(&produttore, NULL, routineProduttore, (void*)&data) != 0){
        perror("Errore creazione produttore\n");
        exit(EXIT_FAILURE);
    }

    if(pthread_create(&consumatore, NULL, routineConsumatore, (void*)&data) != 0){
        perror("Errore creazione consumatore\n");
        exit(EXIT_FAILURE);
    }

    if(pthread_join(produttore, NULL) != 0){
        perror("Errore join produttore\n");
        exit(EXIT_FAILURE);
    }

    if(pthread_join(consumatore, NULL) != 0){
        perror("Errore join consumatore\n");
        exit(EXIT_FAILURE);
    }

    sem_destroy(&data.semProd);
    sem_destroy(&data.semCons);

    printf("Operazioni concluse, arrivederci dal thread: tid\n ");
    sleep(5);

    return 0;
}

