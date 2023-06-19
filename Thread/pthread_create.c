#include "apue.h"
#include <pthread.h>

//Programma che crea un thread e stampa gli ID del processo
//del nuovo thread e del thread del terminale

pthread_t ntid;
void printids(const char *s);
void *thr_fn(void *arg);

int main(void) {
    int err;
    err = pthread_create(&ntid, NULL, thr_fn, NULL);

    if(err != 0) {
        fprintf(stderr, "errore creazione thread: %s \n", strerror(err));
        exit(1);
    }
    printids("thread principale:  ");

    //Senza la sleep il thread principale potrebbe uscire,
    //terminando l’intero processo prima che il nuovo thread abbia
    //la possibilità di iniziare l’esecuzione. Tutto ciò dipende 
    //dall'implementazione dei thread e dagli alg. di scheduling 
    sleep(1);
    exit(0);
}

void printids(const char *s) {
    pid_t pid;
    pthread_t tid;
    pid = getpid();

    //funzione per ottenere il proprio ID senza leggerlo dalla memoria condivisa
    //o riceverlo come argomento alla sua routine di inizio thread
    tid = pthread_self();

    printf("%s pid %d tid %lu (0x%lx)\n", s, (pid_t)pid, (unsigned long)tid, (unsigned long)tid);
}

void *thr_fn(void *arg) {
    printids("new thread:  ");
    return ((void *) 0);
}

