
/*
    • Scrivere un programma che prende in input un intero n, il nome di
        un file di testo ed un carattere x
    • Il programma ha il compito di contare le occorrenze
        del carattere x nel file di testo
    • Il programma esegue tale compito creando n thread, ognuno dei
        quali esamina una porzione diversa del file di testo
    • ad esempio, se il file è lungo 1000 bytes ed n=4, il primo thread
        esaminerà i primi 250 bytes, il secondo thread esaminerà i 250 bytes
        successivi, e così via
*/


#include "apue.h"
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

//Definisco una struttura dati per passare gli argomenti al thread
typedef struct {
    char *filename;
    char search_char;
    int start;
    int end;
    int count;
} threadData;

//Routine per contare le occorrenze di un char 'x' in un file 
void *routine(void *arg) {
    //Passo la struttura come argomento al thread
    threadData* data = (threadData*)arg;

    //Dichiaro file da aprire e verifico corretta apertura
    FILE* file = fopen(data->filename, "r");
    if (file == NULL)
    {
        perror("Errore apertura file");
        pthread_exit(NULL);
    }

    //Posiziono l'indice all'inizio della porzione di file assegnata al thread
    fseek(file, data->start, SEEK_SET);

    //Conto le occorrenze
    int ch;
    while ((ch = fgetc(file)) != EOF && ftell(file) <= data->end)    
    {
        if (ch == data->search_char)
        {
            data->count++;
        }
    }

    fclose(file);
    pthread_exit(NULL);
}

int main() {
    int n;
    char filename[100] = "fileEsercizio2.txt";  
    char search_char; 
    FILE* file;
    int file_length;
    pthread_t* threads;
    threadData* thread_data;
    int i;
    int total_count = 0;

    printf("Inserisci numero di thread:  ");
    scanf(" %d", &n);

    printf("Inserisci il carattere da cercare: ");
    scanf(" %c", &search_char);
    
    // Apertura del file e calcolo della sua lunghezza
    file = fopen(filename, "r");
    if (file == NULL) {
        perror("Errore nell'apertura del file");
        return 1;
    }
    
    fseek(file, 0, SEEK_END);
    file_length = ftell(file);

    fclose(file);

    threads = (pthread_t*)malloc(n*sizeof(pthread_t));

    thread_data = (threadData*)malloc(n*sizeof(threadData));

    int portion_size = file_length/n;

    for(int i = 0; i < n; i++) {
        thread_data[i].filename = filename;
        thread_data[i].search_char = search_char;
        thread_data[i].start = portion_size*i;
        thread_data[i].end = (i == n - 1) ? file_length - 1 : (i + 1) * portion_size - 1;
        thread_data[i].count = 0;

        pthread_create(&threads[i], NULL, routine, (void*)&thread_data[i]);
    }

    for(int i = 0; i < n; i++) {
        pthread_join(threads[i], NULL);
        total_count += thread_data[i].count;
    }

    // Stampa del risultato
    printf("Il carattere '%c' compare %d volte nel file '%s'.\n", search_char, total_count, filename);

    free(threads);
    free(thread_data);

    return 0;
}