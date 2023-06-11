/**
 * Si realizzi un programma C e Posix in ambiente Linux che, impiegando la libreria Pthread, 
 * crei un vettore di interi di dimensione n, dove n>=24 e multiplo di 4,  
 * è un argomento passato da riga di comando.  
 * Calcolati i valori medi degli elementi contenuti nei quattro sottovettori di taglia n/4,  
 * il programma deve determinare, impiegando quattro thread, la somma del numero di elementi minori  
 * del valore medio in ciascun sottovettore.
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

typedef struct {
    int *vettore;
    int n;
    int threadID;
    int sommaLocale;
    int count;
    float media;
} sharedData;

pthread_mutex_t myMutex = PTHREAD_MUTEX_INITIALIZER;

void allocaVettore(sharedData *data){
    //Alloco vettore
    data->vettore = (int*)calloc(data->n, sizeof(int));
    //Riempio vettore
    srand(time(NULL));
    for(int i = 0; i < data->n; i++){
        data->vettore[i] = rand() % 10;
    }
    //Stampo vettore
    for(int i = 0; i < data->n; i++){
        printf("%d  ", data->vettore[i]);
    }
    printf("\n");
}

void *routine(void *arg){
    sharedData *data = (sharedData*)arg;

    int inizio = data->threadID * (data->n / 4);
    int fine = inizio + (data->n / 4);
    int s = 0;
    int numElementiMinori = 0;

    for(int i = inizio; i < fine; i++){
        s += data->vettore[i];
    }

    data->sommaLocale += s;

    data->media = (float)data->sommaLocale / (data->n /4);

    for(int i = inizio; i < fine; i++){
        if(data->vettore[i] < data->media){
            numElementiMinori++;
        }
    }

    pthread_mutex_lock(&myMutex);
    data->count += numElementiMinori;
    pthread_mutex_unlock(&myMutex);

    printf("Thread %d => SottoVettore:  %d => Somma: %d => Media:  %f\n", data->threadID, data->threadID + 1, data->sommaLocale, data->media);
    printf("\t\tSottoVettore:  %d => Numero elementi minori della media:  %d\n", data->threadID + 1, data->count);

    pthread_exit(NULL);
}

int main(int argc, char *argv[]){
    sharedData data;
    if (argc < 2) {
        printf("Inserire il numero di elementi del vettore\n");
        exit(EXIT_FAILURE);
    }
    
    data.n = atoi(argv[1]);

    if(data.n < 24 || data.n%4 != 0){
        printf("Il numero di elementi deve essere n>=24 e un multiplo di 4\n");
        exit(EXIT_FAILURE);
    }

    allocaVettore(&data);

    pthread_t threads[4];
    sharedData thread_data[4];

    for(int i = 0; i < 4; i++){
        thread_data[i].vettore = data.vettore;
        thread_data[i].n = data.n;
        thread_data[i].threadID = i;

        if(pthread_create(&threads[i], NULL, routine, (void*)&thread_data[i]) != 0){
            perror("Errore creazione thread\n");
            exit(EXIT_FAILURE);
        }
    }

    for(int i = 0; i < 4; i++){
        if(pthread_join(threads[i], NULL) != 0){
            perror("Errore join thread\n");
            exit(EXIT_FAILURE);
        }
    }

    return 0;
}