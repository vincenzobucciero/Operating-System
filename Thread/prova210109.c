/**
 * Si realizzi un programma C e Posix in ambiente Linux che, impiegando la libreria Pthread,  
 * crei una matrice di interi di dimensione nxn con n passato come argomento da riga di comando,  
 * e provveda a sommare concorrentemente ciascuna colonna della matrice e a determinarne il minimo il cui valore deve 
 * esser assegnato ad una variabile globale minimo.
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

typedef struct {
    int **matrice;
    int n;
    int threadId;
    pthread_mutex_t myMutex;
} threadData;

int min_value = __INT_MAX__;
int min_thread_id = -1;

void allocaMatrice(threadData *data) {
    printf("Inserisci dimensione matrice:  ");
    scanf("%d", &data->n);

    data->matrice = (int**)calloc(data->n, sizeof(int*));
    for(int i = 0; i < data->n; i++) {
        data->matrice[i] = (int*)calloc(data->n, sizeof(int));
    }
    
    pthread_mutex_init(&data->myMutex, NULL);
}

void riempiMatrice(threadData *data) {
    srand(time(NULL));
    for(int i = 0; i < data->n; i++) {
        for(int j = 0; j < data->n; j++) {
            data->matrice[i][j] = rand() % 10 + 1;
        }
    }
}

void stampaMatrice(threadData *data) {
    printf("Matrice allocata: \n");
    for(int i = 0; i < data->n; i++) {
        for(int j = 0; j < data->n; j++) {
            printf("%d\t", data->matrice[i][j]);
        }
        printf("\n");
    }
}

void deallocaMatrice(threadData *data) {
    for(int i = 0; i < data->n; i++) {
        free(data->matrice[i]);
    }
    free(data->matrice);
}

void *routine(void *arg) {
    threadData *data = (threadData*)arg;
    int s = 0;
    pthread_mutex_lock(&data->myMutex);
    for(int i = 0; i < data->n; i++) {
        s += data->matrice[i][data->threadId];
    }
    pthread_mutex_unlock(&data->myMutex);

    if(s < min_value) {
        min_value = s;
        min_thread_id = data->threadId;
    }

    pthread_exit(NULL);
}

int main() {
    threadData data;

    allocaMatrice(&data);
    riempiMatrice(&data);
    stampaMatrice(&data);

    pthread_t *threads = (pthread_t*)calloc(data.n, sizeof(pthread_t));
    threadData *thread_data = (threadData*)calloc(data.n, sizeof(threadData));

    for(int i = 0; i < data.n; i++) {
        thread_data[i].n = data.n;
        thread_data[i].matrice = data.matrice;
        thread_data[i].threadId = i;

        if(pthread_create(&threads[i], NULL, routine, (void*)&thread_data[i]) != 0) {
            perror("Errore creazione thread\n");
            exit(EXIT_FAILURE);
        }
    }

    for(int i = 0; i < data.n; i++) {
        pthread_join(threads[i], NULL);
    }

    printf("Il valore minimo delle somme delle colonne è: %d\n", min_value);
    printf("Calcolato dal thread con ID: %d\n", min_thread_id);

    deallocaMatrice(&data);

    free(threads);
    free(thread_data);
}