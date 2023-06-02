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

// Direttive al processore
#define MAXVAL 255 // Valore massimo che si può produrre
#define SIZE 6 // Size della matrice quadrata - Numero di Thread

// Variabili globali
int** matrix;
int minimo = MAXVAL * SIZE;

// Mutex
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void allocationMatrix() {
    matrix = ( int** ) calloc( SIZE, sizeof( int* ) );
    for ( int i = 0; i < SIZE; i++ )
        matrix[i] = ( int* ) calloc( SIZE, sizeof( int ) );
}

void initMatrix() {
    for ( int i = 0; i < SIZE; i++ )
        for ( int j = 0; j < SIZE; j++ )
            matrix[i][j] = rand() % MAXVAL + 1;
}

void printMatrix() {
    for ( int i = 0; i < SIZE; i++ ) {
        printf( "%d: ", i );
        for ( int j = 0; j < SIZE; j++ ) {
            int val = matrix[i][j];
            if ( val < 100 && val > 9 )
                printf( "%d     ", val );
            else if ( val < 9 )
                printf( "%d      ", val );
            else 
                printf( "%d    ", val );
        }
        printf( "\n" );
    }
}

void deallocationMatrix() {
    for ( int i = 0; i < SIZE; i++ )
        free( matrix[i] );
    free( matrix );
}

void *routine(void *arg) {
    pthread_t* myTid = ( pthread_t* ) arg;
    printf( "\nMy Tid è: %ld", *myTid );

    // Variabile locale
    int parzialSum = 0;

    if ( *myTid % 2 == 0 ) {
        // Sono pari
        for ( int j = 0; j < SIZE; j += 2 )
            parzialSum += matrix[ *myTid ][j];
    } else {
        // Sono dispari
        for ( int j = 1; j < SIZE; j += 2 )
            parzialSum += matrix[ *myTid ][j];
    }

    // eseguo il controllo del minimo
    // in mutua esclusione essendo soggeto 
    // a race condiction
    pthread_mutex_lock( &mutex );
    if ( parzialSum < minimo )
        minimo = parzialSum;
    pthread_mutex_unlock( &mutex );

    pthread_exit(NULL);
}

int main() {
    srand(time(NULL));
    allocationMatrix();
    initMatrix();
    printf( "\nMatrix:\n" );
    printMatrix();

    pthread_t threads[SIZE];

     // Creazione Thread
    for ( int i = 0; i < SIZE; i++ ) {
        int* tid = ( int* ) malloc( sizeof( int ) );
        *tid = i;
        if ( pthread_create( &threads[i], NULL, routine, (void*) tid ) != 0) {
            perror( "Errore nella creazione dei Thread" );
            exit( EXIT_FAILURE );
        }
    }

    // Attendo i Thread
    for ( int i = 0; i < SIZE; i++ ) {
        if ( pthread_join( threads[i], NULL ) != 0 ) {
            perror( "Errore nella join dei Thread" );
            exit( EXIT_FAILURE );
        }
    }

    printf( "\nIl minimo è: %d", minimo );
    deallocationMatrix();
    printf( "\n" );
    exit( EXIT_SUCCESS );
    return 0;
}
