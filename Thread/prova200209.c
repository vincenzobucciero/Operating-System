/**
 * Si realizzi un programma C e Posix in ambiente Linux che, impiegando la libreria Pthread,  
 * crei una matrice di interi di dimensioni n x n con n numero dispari fornito da riga di comando,  
 * che provveda a sommare in modo concorrente con quattro thread: 
 *  1. gli elementi delle due diagonali;
 *  2. gli elementi della riga centrale;
 *  3. gli elementi della colonna centrale; 
 * e ne determini il massimo da assegnare ad un'opportuna variabile. 
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

typedef struct {
    int **matrice;
    int n;
    pthread_mutex_t myMutex;
} threadData;

int maxSomma = 0;
int sommaDiagDx = 0;
int sommaDiagSx = 0;
int sommaRigaCentr = 0;
int sommaColonnaCentr = 0;

void *sommaDiagonaleDx(void *arg) {
    threadData *data = (threadData*)arg;
    int s = 0;

    pthread_mutex_lock(&data->myMutex);
    for(int i = 0; i < data->n; i++) {
        s += data->matrice[i][data->n - 1 - i];
    }
    pthread_mutex_unlock(&data->myMutex);

    pthread_mutex_lock(&data->myMutex);
    sommaDiagDx += s;
    pthread_mutex_unlock(&data->myMutex);

    if (s > maxSomma) {
        maxSomma = s;
    }

    pthread_exit(NULL);
}

void *sommaDiagonaleSx(void *arg) {
    threadData *data = (threadData*)arg;
    int s = 0;

    pthread_mutex_lock(&data->myMutex);
    for(int i = 0; i < data->n; i++) {
        s += data->matrice[i][i];
    }
    pthread_mutex_unlock(&data->myMutex);

    pthread_mutex_lock(&data->myMutex);
    sommaDiagSx += s;
    pthread_mutex_unlock(&data->myMutex);

    if (s > maxSomma) {
        maxSomma = s;
    }

    pthread_exit(NULL);
}

void *sommaRigaCentrale(void *arg) {
    threadData *data = (threadData*)arg;
    int s = 0;

    pthread_mutex_lock(&data->myMutex);
    for(int j = 0; j < data->n; j++) {
        s += data->matrice[data->n / 2][j];
    }
    pthread_mutex_unlock(&data->myMutex);

    pthread_mutex_lock(&data->myMutex);
    sommaRigaCentr += s;
    pthread_mutex_unlock(&data->myMutex);

    if (s > maxSomma) {
        maxSomma = s;
    }

    pthread_exit(NULL);
}

void *sommaColonnaCentrale(void *arg) {
    threadData *data = (threadData*)arg;
    int s = 0;
    int maxLocal = 0;

    pthread_mutex_lock(&data->myMutex);
    for(int i = 0; i < data->n; i++) {
        s += data->matrice[i][data->n / 2];
    }
    pthread_mutex_unlock(&data->myMutex);

    pthread_mutex_lock(&data->myMutex);
    sommaColonnaCentr += s;
    pthread_mutex_unlock(&data->myMutex);

    if (s > maxSomma) {
        maxSomma = s;
    }

    pthread_exit(NULL);
}

/*
void allocaMatrice(threadData *data) {
    printf("Inserisci dimensione matrice:  ");
    scanf("%d", &data->n);

    data->matrice = (int**)calloc(data->n, sizeof(int*));
    for(int i = 0; i < data->n; i++) {
        data->matrice[i] = (int*)calloc(data->n, sizeof(int));
    }

    pthread_mutex_init(&data->myMutex, NULL);
}
*/

void allocaMatrice(threadData *data, int n) {
    data->n = n;
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
            data->matrice[i][j] = rand()%10+1;
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

int main(int argc, char *argv[]) {
    threadData data;

    if (argc < 2) {
        printf("Utilizzo: %s dimensione_matrice\n", argv[0]);
        return 1;
    }

    int n = atoi(argv[1]);

    if (n % 2 != 1) {
        printf("La dimensione della matrice deve essere un numero dispari.\n");
        return 1;
    }

    allocaMatrice(&data, n);
    riempiMatrice(&data);
    stampaMatrice(&data);

    pthread_t thread1 = (pthread_t)calloc(data.n, sizeof(pthread_t));
    pthread_t thread2 = (pthread_t)calloc(data.n, sizeof(pthread_t));
    pthread_t thread3 = (pthread_t)calloc(data.n, sizeof(pthread_t));
    pthread_t thread4 = (pthread_t)calloc(data.n, sizeof(pthread_t));


    if(pthread_create(&thread1, NULL, sommaDiagonaleDx, (void*)&data) != 0) {
        perror("Errore creazione thread 1\n");
        exit(EXIT_FAILURE);
    }
    if(pthread_create(&thread2, NULL, sommaDiagonaleSx, (void*)&data) != 0) {
        perror("Errore creazione thread 2\n");
        exit(EXIT_FAILURE);
    }
    if(pthread_create(&thread3, NULL, sommaRigaCentrale, (void*)&data) != 0) {
        perror("Errore creazione thread 3\n");
        exit(EXIT_FAILURE);
    }
    if(pthread_create(&thread4, NULL, sommaColonnaCentrale, (void*)&data) != 0) {
        perror("Errore creazione thread 4\n");
        exit(EXIT_FAILURE);
    }
    
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);
    pthread_join(thread3, NULL);
    pthread_join(thread4, NULL);

    printf("Somma diagonale destra: %d\n", sommaDiagDx);
    printf("Somma diagonale sinistra: %d\n", sommaDiagSx);
    printf("Somma riga centrale: %d\n", sommaRigaCentr);
    printf("Somma colonna centrale: %d\n", sommaColonnaCentr);
    printf("\n");
    printf("Somma di Massimo valore: %d\n", maxSomma);

    deallocaMatrice(&data);
}