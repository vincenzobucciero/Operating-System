/**
 *	Da data una matrice nxn( con n pari) di interi generati casualmente, allocata dinamicamente, 
 *  con n argomento da riga di comando, creare n thread che prelevano casualmente un elemento 
 *  della riga di competenza (thread i-esimo, riga i-esima) e lo inseriscano concorrentemente 
 *  in un vettore di (n + 1)/2 elementi.
 *  Un thread n + 1 - esimo attende il riempimento del vettore per stampare il contenuto dello
 *  stesso e per stampare il numero di elementi inseriti nel vettore da ciascun thread. 
 *  Usare mutex e variabili di condizione 
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

typedef struct {
	int **matrice;
	int n;
	int threadId;
} threadData;

pthread_mutex_t myMutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t myCond = PTHREAD_COND_INITIALIZER;

int *vettore;
int *elementiInseriti;
int indexVett = 0;

void allocaMatrice(threadData *data, int n){
	data->n = n;
	data->matrice = (int**)calloc(data->n, sizeof(int*));
	for(int i = 0; i < data->n; i++){
		data->matrice[i] = (int*)calloc(data->n, sizeof(int));
	}
	indexVett = 0;
	vettore = (int*)calloc((data->n + 1) / 2, sizeof(int));
	elementiInseriti = (int*)calloc(data->n, sizeof(int));
}

void riempiMatrice(threadData *data){
	srand(time(NULL));
	for(int i = 0; i < data->n; i++){
		for(int j = 0; j < data->n; j++){
			data->matrice[i][j] = rand()%10;
		}
	}
}

void stampaMatrice(threadData *data){
	for(int i = 0; i < data->n; i++){
		for(int j = 0; j < data->n; j++){
			printf("%d\t", data->matrice[i][j]);
		}
		printf("\n");
	}
}

void deallocaMatrice(threadData *data){
	for(int i = 0; i < data->n; i++){
		free(data->matrice[i]);
	}
	free(data->matrice);
	free(vettore);
	free(elementiInseriti);
}

void *routine(void *arg){
	threadData *data = (threadData*)arg;
	
	int rigaElemento = data->threadId;
	int colonnaElemento = rand()%data->n;
	
	int elemento = data->matrice[rigaElemento][colonnaElemento];
	
	pthread_mutex_lock(&myMutex);
		if (indexVett >= (data->n + 1) / 2) {
        	pthread_cond_signal(&myCond);
        	pthread_mutex_unlock(&myMutex);
        	pthread_exit(0);
    	}
    
		printf("Thread %d: elemento inserito: -> %d\n", data->threadId, elemento);
		vettore[indexVett] = elemento;
		elementiInseriti[data->threadId]++;
		indexVett++;
	pthread_mutex_unlock(&myMutex);
	
	pthread_exit(NULL);
}

void *printElement(void *arg){
	threadData *data = (threadData*)arg;
	
	//mutex per accedere al vettore condiviso
	pthread_mutex_lock(&myMutex);
		
		while (indexVett < (data->n + 1) / 2) {
        	pthread_cond_wait(&myCond, &myMutex);
    	}
    	
    pthread_mutex_unlock(&myMutex);
	
		printf("Contenuto del vettore condiviso:\n");
		for (int i = 0; i < (data->n + 1) / 2; i++) {
			printf("%d ", vettore[i]);
		}
		printf("\n");
	
	pthread_exit(NULL);
}

int main(int argc, char *argv[]){
	threadData data;
	
	if (argc < 2) {
        printf("Utilizzo: %s dimensione_matrice\n", argv[0]);
        return 1;
    }
    
	
	int n = atoi(argv[1]);
	
	if(n % 2 != 0){
		printf("La dimensione della matrice deve essere un numero pari.\n");
        return 1;
	}
	
	allocaMatrice(&data, n);
	riempiMatrice(&data);
	stampaMatrice(&data);
	
	pthread_t *threads = (pthread_t*)calloc(data.n, sizeof(pthread_t));
	threadData *thread_data = (threadData*)calloc(data.n, sizeof(threadData));
	
	for(int i = 0; i < data.n; i++){
		thread_data[i].n = data.n;
        thread_data[i].matrice = data.matrice;
        thread_data[i].threadId = i;
        
        if(pthread_create(&threads[i], NULL, routine, (void*)&thread_data[i]) != 0){
        	perror("Errore creazione thread\n");
        	exit(EXIT_FAILURE);
        }
	}
	
	pthread_t printElementThread;
	if(pthread_create(&printElementThread, NULL, printElement, (void*)&data) != 0){
        	perror("Errore creazione thread\n");
        	exit(EXIT_FAILURE);
    }
	
	for(int i = 0; i < data.n; i++){
		pthread_join(threads[i], NULL);
	}
    
    pthread_join(printElementThread, NULL);
	
	deallocaMatrice(&data);
	
	return 0;
}
