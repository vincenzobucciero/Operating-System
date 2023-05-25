#include "apue.h"
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>


/**
 * @struct char_print_params
 * Qui viene definita una struttura char_print_params 
 * che ha due membri: c, che rappresenta il carattere da stampare, 
 * e count, che rappresenta il numero di volte che il carattere sarà stampato.
*/
struct char_print_params 
{
    char c;
    int count;
};


/**
 * La funzione char_print è il punto focale del programma. 
 * Prende un puntatore generico parameters come argomento, 
 * che verrà successivamente convertito in un puntatore del tipo corretto 
 * struct char_print_params. 
 * All'interno della funzione, viene eseguito un ciclo che stampa 
 * il carattere params->c nel flusso di output di errore (stderr) per params->count volte.
*/
void* char_print(void *parameters) {
    /* cast del puntatore al tipo corretto */
    struct char_print_params *params = (struct char_print_params *)parameters;
    int i;

    for(i=0; i<params->count; i++)
        fputc(params->c, stderr);
    return NULL;
}


int main() {
    pthread_t tid1;
    pthread_t tid2;

    //variabili che verranno utilizzate per passare i parametri ai thread.
    struct char_print_params tid1_args;
    struct char_print_params tid2_args;

    tid1_args.c = 'x';
    tid1_args.count = 100;
    pthread_create(&tid1, NULL, char_print, &tid1_args);

    tid2_args.c = 'y';
    tid2_args.count = 100;
    pthread_create(&tid2, NULL, char_print, &tid2_args);

    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);

    return 0;
}