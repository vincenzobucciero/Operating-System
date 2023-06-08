/**
    Scrivi un programma C in ambiente Linux che simuli il funzionamento di un ponte con due corsie, 
    una in direzione est e l'altra in direzione ovest. 
    
    Il ponte può essere attraversato solo da un'auto per volta e le auto provenienti dalla stessa direzione
    devono seguire un ordine di arrivo.

    Il programma dovrebbe creare due thread produttori, uno per ogni corsia, che generano auto casuali 
    e le fanno attraversare il ponte. 

    Ci sono anche due thread consumatori, uno per ogni corsia, che ricevono le auto che hanno 
    attraversato il ponte e ne stampano i dettagli.

    Il programma deve garantire le seguenti regole:

        - Le auto possono attraversare il ponte solo se non ci sono altre auto che stanno attraversando 
            dalla stessa direzione.
        - Le auto in attesa sulla stessa corsia devono seguire l'ordine di arrivo.
        - Non ci devono essere conflitti nell'accesso al ponte tra i thread.

    Utilizzare i semafori POSIX per garantire la sincronizzazione tra i thread e rispettare le regole di attraversamento del ponte.

    Potrebbe essere utile definire una struttura dati per rappresentare le auto, 
    con campi come la direzione, l'identificatore dell'auto, ecc.
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define N 10

typedef struct {
    int id;
    char direction;
} Car;

int num_producers = 2;
int num_consumers = 2;
int bridge_capacity = 1;
int east_count = 0;
int west_count = 0;
sem_t mutex;
sem_t east_sem;
sem_t west_sem;

void *producer(void *arg) {
    int id = *(int *)arg;
    srand((unsigned int)time(NULL) + id);

    while (1) {
        Car *car = (Car *)malloc(sizeof(Car));
        car->id = id;
        car->direction = (rand() % 2 == 0) ? 'E' : 'W';

        if (car->direction == 'E') {
            sem_wait(&east_sem);
            sem_wait(&mutex);
            east_count++;
            if (east_count == 1) {
                sem_wait(&west_sem);
            }
            sem_post(&mutex);
        } else {
            sem_wait(&west_sem);
            sem_wait(&mutex);
            west_count++;
            if (west_count == 1) {
                sem_wait(&east_sem);
            }
            sem_post(&mutex);
        }

        printf("Car %d (Direction: %c) crossing the bridge.\n", car->id, car->direction);
        sleep(1);

        sem_wait(&mutex);
        if (car->direction == 'E') {
            east_count--;
            if (east_count == 0) {
                sem_post(&west_sem);
            }
        } else {
            west_count--;
            if (west_count == 0) {
                sem_post(&east_sem);
            }
        }
        sem_post(&mutex);

        free(car);
    }

    pthread_exit(NULL);
}

void *consumer(void *arg) {
    while (1) {
        sem_wait(&mutex);
        if (east_count == 0 && west_count == 0) {
            printf("Bridge is empty.\n");
        }
        sem_post(&mutex);

        sleep(2);
    }

    pthread_exit(NULL);
}

int main() {
    pthread_t producers[num_producers];
    pthread_t consumers[num_consumers];

    sem_init(&mutex, 0, 1);
    sem_init(&east_sem, 0, bridge_capacity);
    sem_init(&west_sem, 0, bridge_capacity);

    int producer_ids[num_producers];
    for (int i = 0; i < num_producers; i++) {
        producer_ids[i] = i + 1;
        pthread_create(&producers[i], NULL, producer, (void *)&producer_ids[i]);
    }

    for (int i = 0; i < num_consumers; i++) {
        pthread_create(&consumers[i], NULL, consumer, NULL);
    }

    for (int i = 0; i < num_producers; i++) {
        pthread_join(producers[i], NULL);
    }

    for (int i = 0; i < num_consumers; i++) {
        pthread_join(consumers[i], NULL);
    }

    sem_destroy(&mutex);
    sem_destroy(&east_sem);
    sem_destroy(&west_sem);

    return 0;
}
