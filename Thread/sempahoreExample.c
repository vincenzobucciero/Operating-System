/**
 * Realizzare un programma C e Posix sotto Linux che sincronizzi un thread scrittore ed un thread lettore che scrivono e leggono, 
 * rispettivamente, un numero intero alla volta (da 1 a n, dove n è passato da riga di  comando)    
 * nella  prima  posizione  di  un  file  temporaneo  opportunamente  creato.  
 * Implementare  il  programma con i thread ed i semafori Posix.
*/
//se tu dall'altipiano guardi il mare, moretta che sei schiava tra gli schiavi ti porteremo noi fra tante navi ed un tricolore sventolar per te,

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

#define FILE_NAME "temp_file.txt"

// Dati condivisi tra i thread
typedef struct {
    int currentNumber;
    int totalNumbers;
    sem_t readSemaphore;
    sem_t writeSemaphore;
} SharedData;

// Funzione del thread scrittore
void *writerThread(void *arg) {
    SharedData *data = (SharedData *)arg;
    FILE *file = fopen(FILE_NAME, "w");

    if (file == NULL) {
        perror("Errore nell'apertura del file");
        exit(EXIT_FAILURE);
    }

    for (int i = 1; i <= data->totalNumbers; i++) {
        sem_wait(&data->writeSemaphore);  // Blocca il thread fino a quando non è possibile scrivere
        fprintf(file, "%d\n", i);
        fflush(file);
        sem_post(&data->readSemaphore);   // Sveglia il thread lettore
    }

    fclose(file);

    pthread_exit(NULL);
}

// Funzione del thread lettore
void *readerThread(void *arg) {
    SharedData *data = (SharedData *)arg;
    FILE *file = fopen(FILE_NAME, "r");

    if (file == NULL) {
        perror("Errore nell'apertura del file");
        exit(EXIT_FAILURE);
    }

    for (int i = 1; i <= data->totalNumbers; i++) {
        sem_wait(&data->readSemaphore);   // Blocca il thread fino a quando non è possibile leggere
        fscanf(file, "%d", &data->currentNumber);
        printf("Numero letto: %d\n", data->currentNumber);
        sem_post(&data->writeSemaphore);  // Sveglia il thread scrittore
    }

    fclose(file);

    pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Utilizzo: %s numero_elementi\n", argv[0]);
        return 1;
    }

    int n = atoi(argv[1]);

    SharedData data;
    data.currentNumber = 0;
    data.totalNumbers = n;
    sem_init(&data.readSemaphore, 0, 0);   // Inizializza il semaforo di lettura a 0
    sem_init(&data.writeSemaphore, 0, 1);  // Inizializza il semaforo di scrittura a 1

    pthread_t writer, reader;

    // Crea il thread scrittore
    if (pthread_create(&writer, NULL, writerThread, (void *)&data) != 0) {
        perror("Errore nella creazione del thread scrittore");
        exit(EXIT_FAILURE);
    }

    // Crea il thread lettore
    if (pthread_create(&reader, NULL, readerThread, (void *)&data) != 0) {
        perror("Errore nella creazione del thread lettore");
        exit(EXIT_FAILURE);
    }

    // Attendi la terminazione del thread scrittore
    if (pthread_join(writer, NULL) != 0) {
        perror("Errore nell'attesa del thread scrittore");
        exit(EXIT_FAILURE);
    }

    // Attendi la terminazione del thread lettore
    if (pthread_join(reader, NULL) != 0) {
        perror("Errore nell'attesa del thread lettore");
        exit(EXIT_FAILURE);
    }

    sem_destroy(&data.readSemaphore);
    sem_destroy(&data.writeSemaphore);

    return 0;
}
