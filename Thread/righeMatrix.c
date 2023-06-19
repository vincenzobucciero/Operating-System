/**
 * Si realizzi un programma in C e Posix sotto Linux che, con l’ausilio della libreria Pthread, 
 * lancia n thread per calcolare il prodotto degli elementi di ciascuna riga di una matrice nxn di interi generati 
 * casualmente in un intervallo [0,255]. Il calcolo del prodotto degli elementi di ciascuna riga deve essere
 * effettuato concorrentemente su tutte le righe. Individuati i valori cercati, 
 * si provvederà a ricercarne il massimo ed a stamparlo a video. 
 * La dimensione della matrice può essere fornita in input al programma in fase di esecuzione o da riga di comando.
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

typedef struct {
    int n;
    int **matrice;
    int *prodotto;
    pthread_mutex_t mutex;
} threadData;

void *routine(void* arg) {
    threadData *data = (threadData*)arg;

    for(int i = 0; i < data->n; i++) {
        int prodotto_riga = 1; // Inizializza il prodotto della riga a 1
        for(int j = 0; j < data->n; j++) {
            prodotto_riga *= data->matrice[i][j];
        }
        pthread_mutex_lock(&data->mutex);
        data->prodotto[i] = prodotto_riga; // Salva il prodotto della riga
        pthread_mutex_unlock(&data->mutex);
    }
    pthread_exit(NULL);
}

void allocaMatrice(void* arg) {
    threadData *data = (threadData*)arg;
    printf("Inserisci dimensione matrice:  ");
    scanf("%d", &data->n);

    data->matrice = (int**)calloc(data->n, sizeof(int*));
    for(int i = 0; i < data->n; i++) {
        data->matrice[i] = (int*)calloc(data->n, sizeof(int));
    }

    data->prodotto = (int*)calloc(data->n, sizeof(int));
    for (int i = 0; i < data->n; i++) {
        data->prodotto[i] = 1;
        pthread_mutex_init(&data->mutex, NULL);
    }
}

void riempiMatrice(void* arg) {
    threadData *data = (threadData*)arg;
    srand(time(NULL));
    for(int i = 0; i < data->n; i++) {
        for(int j = 0; j < data->n; j++) {
            data->matrice[i][j] = rand()%256;
        }
        data->prodotto[i] = 1;
    }
}

void stampaMatrice(void* arg) {
    threadData *data = (threadData*)arg;
    for(int i = 0; i < data->n; i++) {
        for(int j = 0; j < data->n; j++) {
            printf("%d\t", data->matrice[i][j]);
        }
        printf("\n");
    }
}

void deallocaMatrice(void* arg) {
    threadData *data = (threadData*)arg;
    for (int i = 0; i < data->n; i++) {
        free(data->matrice[i]);
        pthread_mutex_destroy(&data->mutex);
    }
    free(data->matrice);
    free(data->prodotto);
}

int main() {
    threadData data;

    allocaMatrice(&data);
    riempiMatrice(&data);
    stampaMatrice(&data);

    pthread_t *threads = (pthread_t*)calloc(data.n, sizeof(pthread_t));
    data.prodotto = (int*)calloc(data.n, sizeof(int));

    for (int i = 0; i < data.n; i++) {
        if (pthread_create(&threads[i], NULL, routine, (void*)&data)) {
            perror("Errore creazione thread\n");
            exit(EXIT_FAILURE);
        }
    }

    for (int i = 0; i < data.n; i++) {
        pthread_join(threads[i], NULL);
    }

    int max_prodotto = data.prodotto[0];
    for (int i = 0; i < data.n; i++) {
        if (data.prodotto[i] > max_prodotto) {
            max_prodotto = data.prodotto[i];
        }
        printf("Prodotto riga %d: %d\n", i, data.prodotto[i]);
    }

    printf("Massimo prodotto: %d\n", max_prodotto);

    deallocaMatrice(&data);

    return 0;
}