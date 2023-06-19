/**
 * Si realizzi un programma in C e Posix sotto Linux che, utilizzando la libreria Pthread
 * lancia n thread per cercare un elemento in una matrice nxn di caratteri. 
 * Ognuno dei thread cerca l'elemento in una delle righe della matrice. 
 * Non appena un thread ha trovato l'elemento cercato, rende note agli altri thread le coordinate dell'elemento e tutti i thread terminano.
*/

#include "apue.h"
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

typedef struct {
    int n;
    int **matrice;
    int search_int; //intero da cercare nella matrice 
    int inizio_riga;
    int fine_riga;
    int found;
    int row_index; //indice della riga a cui è stato trovato l'elemento
} threadData;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t condition = PTHREAD_COND_INITIALIZER;

void *routine(void *arg) {
    threadData *data = (threadData*)arg;

    for(int i = 0; i < data->n; i++) {
        if (data->found) {  // Se un altro thread ha già trovato l'elemento, termina
                pthread_exit(NULL);
            }
        for(int j = 0; j < data->n; j++) {
            if(data->matrice[i][j] == data->search_int) {
                pthread_mutex_lock(&mutex);
                if(!data->found) {
                    data->found = 1;
                    data->row_index = i;
                }
                pthread_mutex_unlock(&mutex);
                pthread_exit(NULL);
            }
        }
    }
    pthread_exit(NULL);
}

int main() {
    int n;
    int search_int;

    printf("Inserisci dimensione matrice:  ");
    scanf("%d", &n);

    int **matrice = (int**)calloc(n, sizeof(int*));
    for(int i = 0; i < n; i++) {
        matrice[i] = (int*)calloc(n, sizeof(int));
    }

    srand(time(NULL));
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < n; j++) {
            matrice[i][j] = rand()%10+1;
        }
    }

    for(int i = 0; i < n; i++) {
        for(int j = 0; j < n; j++) {
            printf("%d\t", matrice[i][j]);
        }
        printf("\n");
    }

    printf("Inserisci elemento da cercare:  ");
    scanf("%d", &search_int);

    pthread_t *threads = (pthread_t*)calloc(n, sizeof(pthread_t));
    threadData *data = (threadData*)calloc(n, sizeof(threadData));

    for(int i = 0; i < n; i++) {
        data[i].n = n;
        data[i].matrice = matrice;
        data[i].search_int = search_int;
        data[i].inizio_riga = i;
        data[i].fine_riga = i;
        data[i].found = 0;
        data[i].row_index = -1;

        if(pthread_create(&threads[i], NULL, routine, (void*)&data[i])) {
            perror("Errore creazione thread\n");
            exit(EXIT_FAILURE);
        }
    }

    for (int i = 0; i < n; i++) {
        pthread_join(threads[i], NULL);
    }

    for (int i = 0; i < n; i++) {
        pthread_cancel(threads[i]);
    }

    int elemento_trovato = 0;
    int riga_trovata = -1;

    for (int i = 0; i < n; i++) {
        if (data[i].found) {
            elemento_trovato = 1;
            riga_trovata = data[i].row_index;
            break;
        }
    }

    if (elemento_trovato) {
        printf("Elemento trovato nella riga %d\n", riga_trovata);
    } else {
        printf("Elemento non trovato\n");
    }

    free(threads);
    free(data);
    for (int i = 0; i < n; i++) {
        free(matrice[i]);
    }
    free(matrice);

    return 0;
}