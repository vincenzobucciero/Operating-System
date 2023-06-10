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

int n;
int *vettore;
float media;

pthread_mutex_t myMutex = PTHREAD_MUTEX_INITIALIZER;

void *routine(void *arg){
    int id = *(int*)arg;
    printf("Thread ID: %d\n", id);

    int inizio = id * (n/4);
    int fine = inizio + (n/4);

    for(int i = inizio; i < fine; i++){
        
    }
}

int main(int argc, char *argv[]){
    if(argc < 2){
        perror("Errore\n");
        exit(EXIT_FAILURE);
    }

    n = atoi(argv[1]);

    if(n < 24 || n%4 != 0){
        perror("Devi inserisce un numero n>=24 e multiplo di 4..\n");
        exit(EXIT_FAILURE);
    }

    vettore = (int*)malloc(n*sizeof(int));

    srand(time(NULL));
    for(int i = 0; i < n; i++){
        vettore[i] = rand()%10;
    }

    for(int i = 0; i < n; i++){
        printf("%d  ", vettore[i]);
    }

    printf("\n");

    media = 


    return 0;
}