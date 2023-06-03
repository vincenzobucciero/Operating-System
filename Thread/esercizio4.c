/**
 * ESERCIZIO: 4
 * 
 * Si realizzi un programma in C e Posix sotto Linux che,
 * con l’ausilio della libreria Pthread
 * 
 * • lancia n thread per calcolare la somma degli elementi
 *   di ciascuna riga di una matrice nxn di interi generati
 *   casualmente in un intervallo [0,255], allocata dinamicamente
 * 
 * • Il calcolo della somma degli elementi di ciascuna riga
 *   deve essere effettuato concorrentemente su tutte le 
 *   righe, secondo la seguente modalità:
 *      
 *    -> il thread i-esimo con indice i pari, calcola la somma degli 
 *       elementi di indice pari della riga i-esima
 * 
 *    -> il thread con indice i dispari, calcola la somma degli
 *       elementi di indice dispari della riga i-esima.
 * 
 * • Calcolate le somme parziali, si provvederà a ricercarne il minimo ed a stamparlo a video.
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
int minSommaParziale = __INT_MAX__;

typedef struct {
    int n;
    int **matrice;
    int threadIndex;
} threadData;

void *routine(void *arg) {
    threadData *data = (threadData *)arg;
    int index = data->threadIndex;

    data->sommaParziale = 0;
    int *row = data->matrice[index];

    // Array dinamico per gli elementi sommati
    int *elementiSommati = malloc(data->n * sizeof(int));
    int count = 0;

    if(index % 2 == 0) {
        for(int i = 0; i < data->n; i+=2) {
            data->sommaParziale += row[i];
            elementiSommati[count] = row[i];
            count++;
        }
    } else {
        for(int i = 1; i < data->n; i+=2) {
            data->sommaParziale += row[i];
            elementiSommati[count] = row[i];
            count++;
        }
    }

    printf("Somma parziale riga %d -> Elementi sommati : ", index);
    for(int i = 0; i < count; i++) {
        printf(" -> %d ", elementiSommati[i]);
    }
    printf("\n");
    printf("Somma parziale riga %d -> %d\n\n", index, data->sommaParziale);

    pthread_mutex_lock(&mutex);
    if(data->sommaParziale < minSommaParziale) {
        minSommaParziale = data->sommaParziale;
    }
    pthread_mutex_unlock(&mutex);

    // Restituisci la somma come risultato del thread
    int *result = malloc(sizeof(int));
    *result = data->sommaParziale;

    pthread_exit(result);
}

int main() {
    int n;

    printf("Inserisci dimensione matrice:  ");
    scanf("%d", &n);

    //Allocazione matrice
    int **matrice = (int**)malloc(n*sizeof(int*));
    for(int i = 0; i < n; i++) {
        matrice[i] = (int*)malloc(n*sizeof(int));
    }

    //Assegno valori casuali alla matrice
    srand(time(NULL));
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < n; j++) {
            matrice[i][j] =rand()%10+1;
        }
    }

    //Stampa della matrice
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < n; j++) {
            printf("%d\t",matrice[i][j]);
        }
        printf("\n");
    }

    pthread_t threads[n];
    threadData data[n];

    //Creo thread
    for(int i = 0; i < n; i++) {
        data[i].n = n;
        data[i].matrice = matrice;
        data[i].threadIndex = i;

        if(pthread_create(&threads[i], NULL, routine, (void*)&data[i]) != 0) {
            printf("Errore nella creazione del thread.\n");
            exit(EXIT_FAILURE);
        }
    }

    //Attendere la terminazione dei thread e raccogliere i risultati
    //e Calcolo somma parziale minima tra tutti i thread
    for (int i = 0; i < n; i++) {
        int *result;
        if (pthread_join(threads[i], (void*)&result) != 0) {
            printf("Errore nella join del thread.\n");
            exit(EXIT_FAILURE);
        }

        free(result);
    }

    printf("La somma minima è: %d\n", minSommaParziale);

    return 0;
}