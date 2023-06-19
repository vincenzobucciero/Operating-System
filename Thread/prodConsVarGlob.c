/**
 * Scrivere un programma C in ambiente Linux che facendo uso della libreria Pthread realizzi il seguente comportamento: 
 * 
 * un master thread inizializza una variabile globale a zero, crea un thread produttore ed un thread consumatore e, 
 * in un ciclo infinito, stampa il valore della variabile globale.
 *   
 * Il produttore incrementa, ad ogni passo, la variabile globale di due unità e dorme per un secondo. 
 * 
 * Il consumatore decrementa la variabile globale di una unità e dorme per un secondo. 
 * 
 * Il processo termina quando la variabile globale è maggiore di un valore intero passato da riga di comando. 
 * 
 * Utilizzare un semaforo Posix per la sincronizzazione.
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

int var_glob = 0;
int n;

sem_t sem_prod, sem_cons;

void *threadProduttore(void *arg){
    while(1){
        sem_wait(&sem_prod);
        if(var_glob > n){
            sem_post(&sem_cons);
            sem_post(&sem_prod);
            printf( "\nFine produttore" );
            pthread_exit(NULL);
        }
        printf( "\nProdotto 2, glob -> %d", var_glob );
        var_glob += 2;
        sem_post(&sem_cons);
        sem_post(&sem_prod);
        sleep(1);
    }
    pthread_exit(NULL);
}

void *threadConsumatore(void *arg){
    while(1){
        sem_wait(&sem_cons);
        sem_wait(&sem_prod);
        if(var_glob > n){
            sem_post(&sem_prod);
            sem_post(&sem_cons);
            printf( "\nFine consumatore" );
            pthread_exit(NULL);
        }
        printf( "\nConsumato 1, glob -> %d", var_glob );
        var_glob --;
        sem_post(&sem_prod);
        sem_post(&sem_cons);
        sleep(1);
    }
    pthread_exit(NULL);
}

int main(int argc, char *argv[]){

    if(argc < 2){
        printf("Inserire un numero intero\n");
        return -1;
    }

    n = atoi(argv[1]);

    sem_init(&sem_prod, 0, 1);
    sem_init(&sem_cons, 0, 0);

    pthread_t produttore, consumatore;

    pthread_create(&produttore, NULL, threadProduttore, NULL);
    pthread_create(&consumatore, NULL, threadConsumatore, NULL);

    pthread_join(produttore, NULL);
    pthread_join(consumatore, NULL);

    sem_destroy(&sem_prod);
    sem_destroy(&sem_cons);

    printf( "\n" );
    return 0;
}