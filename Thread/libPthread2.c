#include "apue.h"
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h> 

#define NUM_THREADS 8

/* Questa routine riceve in ingresso l'id del thread */
void *printHello(void *threadId) {
    int *id_ptr, taskid;

    sleep(1);

    /*
        Il puntatore generico threadid viene reinterpretato come un puntatore 
        a intero int * e viene assegnato a id_ptr. Successivamente, 
        il valore puntato da id_ptr viene assegnato a taskid. 
        Questo viene fatto per estrarre l'ID del thread passato come argomento.
    */
    id_ptr = (int *) threadId;
    taskid = *id_ptr;

    printf("Thread  %d\n", taskid);

    pthread_exit(NULL);
}

int main() {
    pthread_t threads[NUM_THREADS];
    int *taskids[NUM_THREADS];
    int rc, t;

    for(t=0; t<NUM_THREADS; t++) {
        taskids[t] = (int *) malloc(sizeof(int));
        *taskids[t] = t;
        printf("Creating thread %d\n", t);
        rc = pthread_create(&threads[t], NULL, printHello, (void *) taskids[t]);

        if (rc)
        {
            printf("ERROR, return code from pthread_create() is %d\n", rc);
            exit(-1);
        }
        
    }
    pthread_exit(NULL);
}