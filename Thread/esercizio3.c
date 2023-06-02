//NON E' IN REALTA' ESERCIZIO 3, SEGUIRE SLIDE

/*
    Si realizzi un programma C e Posix sotto Linux che, utilizzando la libreria Pthread:
        - lancia n thread per cercare un elemento in una matrice nxn di interi
        - ognuno dei thread cerca l'elemento in una delle righe della matrice
        - non appena un thread ha trovato l'elemento cercato, rende noto agli altri thread
            le coordinate dell'elemento e tutti i thread terminano(sono cancellati).
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

typedef struct {
    int n;
    int **matrix;
    int search;
    int found;
    int row_found;
    pthread_mutex_t mutex;
} ThreadData;

void *searchElement(void *arg) {
    ThreadData *data = (ThreadData *)arg;

    for (int i = 0; i < data->n; i++) {
        if (data->found)
            pthread_exit(NULL); // Se l'elemento è stato trovato da un altro thread, termina

        for (int j = 0; j < data->n; j++) {
            if (data->matrix[i][j] == data->search) {
                pthread_mutex_lock(&data->mutex);

                if (!data->found) { // Verifica se l'elemento è stato trovato da un altro thread
                    data->found = 1;
                    data->row_found = i;
                    printf("Elemento %d trovato alle coordinate (%d, %d)\n", data->search, i, j);
                }

                pthread_mutex_unlock(&data->mutex);
                pthread_exit(NULL);
            }
        }
    }

    pthread_exit(NULL);
}

int main() {
    int n;
    int search;

    printf("Inserisci n per dimensione della matrice: ");
    scanf("%d", &n);

    int **matrix = malloc(n * sizeof(int *));
    for (int i = 0; i < n; i++) {
        matrix[i] = malloc(n * sizeof(int));
    }

    srand(time(NULL));
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            matrix[i][j] = rand() % 10 + 1;
        }
    }

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            printf("%d\t", matrix[i][j]);
        }
        printf("\n");
    }

    printf("Inserisci elemento da cercare: ");
    scanf("%d", &search);

    pthread_t *threads = malloc(n * sizeof(pthread_t));
    ThreadData *threadData = malloc(n * sizeof(ThreadData));
    pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

    for (int i = 0; i < n; i++) {
        threadData[i].n = n;
        threadData[i].matrix = matrix;
        threadData[i].search = search;
        threadData[i].found = 0;
        threadData[i].row_found = -1;
        threadData[i].mutex = mutex;

        pthread_create(&threads[i], NULL, searchElement, &threadData[i]);
    }

    for (int i = 0; i < n; i++) {
        pthread_join(threads[i], NULL);
    }

    for (int i = 0; i < n; i++) {
        free(matrix[i]);
    }
    free(matrix);
    free(threads);
    free(threadData);

    return 0;
}
