/**
 * ESERCIZIO: 1
 * 
 * Si realizzi un programma in C e Posix sotto Linux 
 * che, utilizzando la libreria Pthread:
 * 
 * • lancia n thread per cercare un elemento in una matrice nxn di interi
 * 
 * • Ognuno dei thread cerca l'elemento in una delle righe della matrice
 * 
 * • Non appena un thread ha trovato l'elemento cercato, 
 *   rende note agli altri thread le coordinate dell'elemento
 *   e tutti i thread terminano (sono cancellati)
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

#define TRUE 0
#define FALSE 1

// Variabili globali
int** matrix;
int sizeMatrix;
int findNumber;
int row = - 1, col = - 1;
int created = FALSE;
int find = FALSE;

// Mutex e variabile condizione
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

// Thread
pthread_t* th;

void initMatrix() {
    for ( int i = 0; i < sizeMatrix; i++ )
        for ( int j = 0; j < sizeMatrix; j++ )
            matrix[i][j] = rand() % 10;
}

void printMatrix() {
    printf( "\n" );
    for ( int i = 0; i < sizeMatrix; i++ ) {
        for ( int j = 0; j < sizeMatrix; j++ )
            printf( "%d    ", matrix[i][j] );
        printf( "\n" );
    }
}

void allocationMatrix() {
    matrix = ( int** ) calloc( sizeMatrix, sizeof( int* ) );
    for ( int i = 0; i < sizeMatrix; i++ )
        matrix[i] = ( int* ) calloc( sizeMatrix, sizeof( int ) );
}

void deAllocationMatrix() {
    for ( int i = 0; i < sizeMatrix; i++ )
        free( matrix[i] );
    free( matrix );
}

/**
 * routine dei Thread
 * il Thread deve cercare per la sua riga il valore richiesto in input
 * dopodiche se trovato eliminare tutti gli altri Thread,
 * viene eseguito un primo controllo in mutua esclusione per verificare
 * se l'elemento anche se trovato, gia è stato trovato allora prosegue senza fare ulteriori operazioni
 * altrimenti se è stato il primo a trovarlo procede, aspettando che prima tutti i Thread siano stati creati,
 * questo perchè se procede con la cancellazzione dei Thread ma in realtà non ancora tutti i Thread sono stati cancellati
 * allora si imbatte in un problema di segmentation fault, quindi una volta creati tutti i Thread provvede a cancellarli.
 * 
 * @param argv tid del Thread
*/
void* routine( void* argv ) {

    pthread_t* myTid = ( pthread_t* ) argv;
    printf( "\nMy Tid è: %ld", *myTid );

    // Inizio la mia ricerca
    for ( int j = 0; j < sizeMatrix; j++ ) {

        // Se l'elemento non è stato ancora trovato
        pthread_mutex_lock( &mutex );
        if ( find == FALSE ) {
            pthread_mutex_unlock( &mutex );

            // Se ho trovato l'elemento
            if ( matrix[ *myTid ][j] == findNumber ) {

                // Imposto la flag dell'elemento trovato
                pthread_mutex_lock( &mutex );
                find = TRUE;
                pthread_mutex_unlock( &mutex );

                /**
                 * Prima di cancellare tutti i thread
                 * è necessario aspettare che tutti vengano creati
                 * in modo tale da non accedere a memorie inesistenti
                 * e quindi di generare un segmentation fault
                 * 
                 * Utilizzo una variabile condizione in modo da evitare 
                 * l'utilizzo della tecnica Polling
                */
                while ( created == FALSE )
                    pthread_cond_wait( &cond, &mutex );

                // Salvo i punti
                row = *myTid;
                col = j;

                // Cancello tutti i Thread
                for ( int i = 0; i < sizeMatrix; i++ ) {
                    if ( i != *myTid ) {
                        if ( pthread_cancel( th[i] ) == 0 )
                            printf( "\nThread %ld cancella %d", *myTid, i );
                        else 
                            printf( "\nThread %d non esiste", i );
                    }
                }
                pthread_mutex_unlock( &mutex );
                pthread_exit( NULL );
            }
        } else
            pthread_mutex_unlock( &mutex );
    }
    pthread_exit( NULL );
}

int main( int argc, char* argv[] ) {

    // Prendo come seme l'orario attuale
    srand( time( NULL ) );

    if ( argc != 2 ) {
        perror( "Errore nei parametri in input" );
        exit( EXIT_FAILURE );
    }

    sizeMatrix = atoi( argv[1] );
    printf( "\nMatrix sarà di %d righe ed %d colonne", sizeMatrix, sizeMatrix );

    // Allocazione, inizializzazione ed stampa Matrix
    allocationMatrix();
    initMatrix();
    printMatrix();

    printf( "Inserisci il numero da cercare: " );
    scanf( "%d", &findNumber );

    // Thread
    th = calloc ( sizeMatrix, sizeof( pthread_t ) );

    // Creazione Thread
    for ( int i = 0; i < sizeMatrix; i++ ) {
        int* tid = malloc( sizeof( int* ) );
        *tid = i;
        if ( pthread_create( &th[i], NULL, routine, ( void* ) tid ) != 0 ) {
            perror( "Errore nella creazione dei Thread" );
            exit( EXIT_FAILURE );
        }
    }
    
    printf( "\nTutti i Thread sono stati creati." );
    pthread_mutex_lock( &mutex );
    created = TRUE;
    // Avviso il Thread che sta aspettando che può proseguire
    pthread_cond_signal( &cond );
    pthread_mutex_unlock( &mutex );

    // Attesa Thread
    for ( int i = 0; i < sizeMatrix; i++ ) {
        if ( pthread_join( th[i], NULL ) != 0 ) {
            perror( "Errore nella join dei Thread" );
            exit( EXIT_FAILURE );
        }
    }

    if ( row != - 1 && col != -1 )
        printf( "\n\nRiga: %d, Colonna: %d", row, col);
    else 
        printf( "\n\nNumero non trovato!" );

    // Deallocazione ed uscita dal programma
    deAllocationMatrix();
    printf( "\n" );
    exit( EXIT_SUCCESS );
}