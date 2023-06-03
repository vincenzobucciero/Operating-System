/**
 * ESERCIZIO: 6
 * 
 * Scrivere un programma C e Posix sotto Linux che,
 * utilizzando la libreria Pthread crei due Thread, chiamati produttore e consumatore.
 * La risorsa condivisa, è un buffer circolare di dimensione data
 * ( ad esempio 20 ) il cui stato è:
 * 
 * • numero di elemento contenuti: count
 * • puntatore alla prima posizione libera: writepos
 * • puntatore al primo elemento occupato: readpos
 * 
 * Il produttore, inserisce, 20 numeri interi in maniera sequenziale.
 * 
 * Il consumatore li estrae sequenzialmente per stamparli.
 * 
 * Il programma dovrà prevedere:
 * 
 * • un meccanismo di accesso controllato alla risorsa buffer da parte dei due Thread
 *  (mutex per il controllo della mutua esclusione nell'accesso al buffer)
 * 
 * • una sincronizzazione tra il produttore ed il consumatore (Thread) in caso di
 * 
 *      -> buffer pieno: definizione di una condition per la sospensione del 
 *         produttore se il buffer è pieno (notfull)
 * 
 *      -> buffer vuoto: definizione di una condition per la sospensione del
 *         consumatore se il buffer è vuoto (notempty)
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

#define BUFFER_SIZE 20

int count = 0;
int writepos = 0;
int readpos = 0;
int buffer[BUFFER_SIZE];

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

pthread_cond_t notfull = PTHREAD_COND_INITIALIZER;
pthread_cond_t notempty = PTHREAD_COND_INITIALIZER;

void *routineProduttore(void *arg) {
    int i;

    for(i = 0; i < BUFFER_SIZE; i++) {
        pthread_mutex_lock(&mutex);
        while(count == BUFFER_SIZE) 
            pthread_cond_wait(&notfull, &mutex);
        buffer[writepos] = i;
        writepos = (writepos+1)%BUFFER_SIZE;
        count++;

        printf("Insert: %d\n", i);

        pthread_cond_signal(&notempty);

        pthread_mutex_unlock(&mutex);
    }
    pthread_exit(NULL);
}

void *routineConsumatore(void *arg) {
    int i;
    for(i = 0; i < BUFFER_SIZE; i++) {
        pthread_mutex_lock(&mutex);
        while(count == 0)
            pthread_cond_wait(&notempty, &mutex);
        int x = buffer[readpos];
        readpos = (readpos+1)%BUFFER_SIZE;
        count--;

        printf("Consumed: %d\n", x);

        pthread_cond_signal(&notfull); 

        pthread_mutex_unlock(&mutex);
    }
    pthread_exit(NULL);
}

int main() {
    pthread_t prod_thread, cons_thread;

    // Creazione del thread produttore
    if(pthread_create(&prod_thread, NULL, routineProduttore, NULL) != 0) {
        perror("ERRORE CREAZIONE");
        exit(EXIT_FAILURE);
    }
    
    // Creazione del thread consumatore
    if(pthread_create(&cons_thread, NULL, routineConsumatore, NULL) != 0) {
        perror("ERRORE CREAZIONE");
        exit(EXIT_FAILURE);
    }

    // Attesa della terminazione del thread produttore
    if(pthread_join(prod_thread, NULL) != 0) {
        perror("ERRORE JOIN");
        exit(EXIT_FAILURE);
    }
    
    // Attesa della terminazione del thread consumatore
    if(pthread_join(cons_thread, NULL) != 0) {
        perror("ERRORE JOIN");
        exit(EXIT_FAILURE);
    }

    return 0;
}