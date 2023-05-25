#include "apue.h"
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

void* thread1(void *arg) {
    int error;
    error = *(int*)arg;
    printf("Sono il primo thread. Parametro = %d\n", *(int*)arg);

    pthread_exit((void*)(long)error);
}

void* thread2(void *arg) {
    static long error;
    error=*(int*)(arg);
    printf("Sono il secondo thread. Parametro = %d\n",(int)error);
    
    pthread_exit((void*)&error);
}

int main() {
    pthread_t th1, th2;
    int i1 = 1, i2=2;
    void* uscita;

    pthread_create(&th1, NULL, thread1,(void*)&i1);
    pthread_create(&th2, NULL, thread2,(void*)&i2);

    pthread_join(th1, &uscita);

    printf("stato = %ld\n", (long)uscita);

    pthread_join(th2, &uscita);

    printf("stato = %ld\n", *(long*)uscita);
    
    exit(0);
}