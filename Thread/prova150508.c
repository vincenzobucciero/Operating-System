/**
 * Si realizzi un programma in C e Posix sotto Linux che, con l’ausilio della libreria Pthread, 
 * lancia n thread per calcolare la somma dei reciproci degli elementi di ciascuna riga di una matrice nxn 
 * di interi generati casualmente in un intervallo [0,255]. 
 * 
 * Il calcolo della somma dei reciproci degli elementi di ciascuna riga deve essere effettuato concorrentemente su tutte le righe. 
 *
 * Individuati i valori cercati, si provvederà a ricercarne il minimo ed a stamparlo a video. 
 * 
 * La dimensione della matrice può essere fornita in input al programma in fase di esecuzione o da riga di comando. 
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <limits.h>

typedef struct {
    int n;
    int **matrice;
    pthread_mutex_t myMutex;
    int threadId;
} threadData;

double min_value = INT_MAX;
int min_thread_id = -1;

/**
 * Per il calcolo della somma sulle righe
 * Ad esempio, se hai una matrice 3x4 come questa:
 *              1 2 3 4
 *              5 6 7 8
 *              9 8 7 6
 * Bisogna calcolare la somma dei reciproci degli elementi di ogni riga separatamente. 
 * Quindi, per la prima riga, calcola 1/1 + 1/2 + 1/3 + 1/4, 
 * per la seconda riga calcola 1/5 + 1/6 + 1/7 + 1/8, e così via.
*/
void *routine(void* arg) {
    threadData *data = (threadData*)arg;
    double s = 0.0;
    pthread_mutex_lock(&data->myMutex);
    for (int i = 0; i < data->n; i++) {
        s += 1.0 / data->matrice[data->threadId][i];
    }
    pthread_mutex_unlock(&data->myMutex);
    if (s < min_value) {
        min_value = s;
        min_thread_id = data->threadId;
    }
    
    pthread_exit(NULL);
}

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
            data->matrice[i][j] = rand()%256;
        }
    }
}

void stampaMatrice(threadData *data) {
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
    
    pthread_mutex_destroy(&data->myMutex);
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

    for (int i = 0; i < data.n; i++) {
        pthread_join(threads[i], NULL);
    }

    printf("Il valore minimo delle somme dei reciproci è: %lf\n", min_value);
    printf("Calcolato dal thread con ID: %d\n", min_thread_id);

    deallocaMatrice(&data);

    free(threads);
    free(thread_data);

    return 0;
}