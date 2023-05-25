#include "apue.h"
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h> 

//function per la stampa del messaggio
void *print_message (void *ptr)
{
    char *message;
    message = (char *) ptr;

    printf("%s \n", message);
}

int main() {
    pthread_t tid1, tid2;

    char *message1 = "Hello World";
    char *message2 = "Hello World 2";

    //variabili per la stampa dei thread
    int iret1, iret2;

    iret1 = pthread_create(&tid1, NULL, print_message, (void *)message1);    
    iret2 = pthread_create(&tid2, NULL, print_message, (void *)message2);

    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);

    printf("Thread 1 return:  %d\n", iret1);
    printf("Thread 2 return:  %d\n", iret2);

    exit(0);
}