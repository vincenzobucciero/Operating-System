/**
 * Si realizzi un programma C e Posix in ambiente Linux che, impiegando la libreria Pthread, 
 * crei un vettore di interi di dimensione n, dove n>=24 e multiplo di 4, è un argomento passato da riga di comando.  
 * Calcolati i valori medi degli elementi contenuti nei quattro sottovettori di taglia n/4,  
 * il programma deve determinare, impiegando quattro thread, la somma del numero di elementi minori del  
 * valore medio in ciascun sottovettore.
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

typedef struct {
    int *vettore;
    int n;
    int threadId;
    int sommaMinoriMedia;
} threadData;

void *routine(void *arg){
    threadData *data = (threadData*) arg;
    int s = 0;
    int avg = 0;
    
    for (int i = 0; i < data->n; i++) {
        avg += data->vettore[data->threadId * (data->n / 4) + i];
    }
    avg = data->n / 4;

    for(int i = 0; i < data->n; i++){
        if (data->vettore[data->threadId * (data->n / 4) + i] < avg) {
            s++;
        }
    }

    data->sommaMinoriMedia = s;

    pthread_exit(NULL);
}

void allocaVettore(threadData *data, int n){
    data->n = n;
    data->vettore = (int*)calloc(data->n, sizeof(int));

    srand(time(NULL));
    for (int i = 0; i < data->n; i++) {
        data->vettore[i] = rand() % 10;
    }

    for (int i = 0; i < data->n; i++) {
        printf("%d\t", data->vettore[i]);
    }
    
    printf("\n");

}

int main(int argc, char *argv[]){
    threadData data;

    if (argc < 2) {
        printf("Utilizzo: %s dimensione_matrice\n", argv[0]);
        return 1;
    }
    
	int n = atoi(argv[1]);
	
	if(n < 24 && n % 4 == 0){
		printf("La dimensione della matrice deve avere una dimensione >= 24 e deve essere un multiplo di 4.\n");
        return 1;
	}

    allocaVettore(&data, n);
    
    threadData thread_data[4];
    pthread_t threads[4];

    // Creazione dei thread
    for (int i = 0; i < 4; i++) {
        thread_data[i].threadId = i;
        thread_data[i].n = data.n;
        thread_data[i].vettore = data.vettore;
        thread_data[i].sommaMinoriMedia = data.sommaMinoriMedia;
        if (pthread_create(&threads[i], NULL, routine, (void*)&thread_data[i]) != 0) {
            perror("Errore creazione thread\n");
            exit(EXIT_FAILURE);
        }
    }

    // Attesa dei thread
    for (int i = 0; i < 4; i++) {
        pthread_join(threads[i], NULL);
    }

    // Stampa delle somme dei numeri minori della media in ciascun sottovettore
    for (int i = 0; i < 4; i++) {
        printf("Somma sottovettore %d: %d\n", i + 1, thread_data[i].sommaMinoriMedia);
    }

    return 0;

}