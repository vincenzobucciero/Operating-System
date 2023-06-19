/**
 * Si realizzi un programma C e Posix in ambiente Linux che, impiegando la libreria Pthread, 
 * crei un vettore di interi di dimensione n, dove n>=21 e multiplo di 3,  
 * è un argomento passato da riga di comando.  
 * Calcolato il valore medio degli elementi contenuti nel vettore,    
 * il programma deve determinare, impiegando tre thread, il numero di elementi maggiori 
 * del valore medio, rispettivamente nei tre sottovettori di dimensioni n/3
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

int n;
int *vettore;

pthread_mutex_t myMutex = PTHREAD_MUTEX_INITIALIZER;

int count = 0;
float media;

void *routine(void *arg){
    //id del thread
    int id = *(int*)arg;

    printf("Thread %d: ciao stronzo\n", id);

    //indice di partenza del sottovettore associato al thread 
    int inizio = id * (n/3);

    //indice di fine del sottovettore
    int fine = inizio + (n/3);

    int numElemMaggiori = 0; // Conteggio locale degli elementi maggiori della media

    // Itera sui elementi del sottovettore
    for (int i = inizio; i < fine; i++) {
        if (vettore[i] > media) {
            numElemMaggiori++; // Incrementa il conteggio locale se l'elemento è maggiore della media
        }
    }

    // Aggiorna il conteggio globale in modo sicuro utilizzando un mutex
    pthread_mutex_lock(&myMutex);
    count += numElemMaggiori;
    pthread_mutex_unlock(&myMutex);

    pthread_exit(NULL); // Termina il thread
}

int main(int argc, char *argv[]){
    if(argc < 2){
        printf("Inserire un numero in input\n");
        exit(EXIT_FAILURE);
    }

    n = atoi(argv[1]);

    if(n < 21 && n%3 != 0){
        printf("Inserire un numero maggiore o uguale di 21 e multiplo di 3\n");
        exit(EXIT_FAILURE);
    }

    //vettore = (int*)calloc(n, sizeof(int));

    vettore = (int*)malloc(n*sizeof(int));

    srand(time(NULL));
    for(int i = 0; i < n; i++){
        vettore[i] = rand()%51;
    }

    for(int i = 0; i < n; i++){
        printf("%d\t", vettore[i]);
    }

    printf("\n");

    int somma = 0;
    for(int i = 0; i < n; i++){
        somma += vettore[i];
    }
    printf("Somma:  %d\n", somma);

    media = (float)somma/n;

    printf("Media:  %f\n", media);

    pthread_t threads[3];
    int threadIDs[3];

    for(int i = 0; i < 3; i++){
        threadIDs[i] = i;
        if(pthread_create(&threads[i], NULL, routine, &threadIDs[i]) != 0){
            perror("Errore creazione thread\n");
            exit(EXIT_FAILURE);
        }
    }

    for(int i = 0; i < 3; i++){
        if(pthread_join(threads[i], NULL) != 0){
            perror("Errore join thread\n");
            exit(EXIT_FAILURE);
        }
    }

    printf("Numero di elementi maggiori della media: %d\n", count);

    free(vettore);

    return 0;
}