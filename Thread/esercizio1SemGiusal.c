/**
 *  Facendo uso della libreria Pthread, realizzare un programma in cui un
 *  thread scrittore, dato un intero N da riga di comando (dove
 *  10<N<=15), scrive in un file nella prima posizione, uno alla volta ed
 *  ogni ½ secondo, la sequenza di Fibonacci di ordine N, alternandosi con
 *  un thread lettore che legge, uno alla volta dalla prima posizione del
 *  file, i numeri scritti dal thread scrittore. Un terzo thread attende la
 *  lettura dell’ N-esimo intero, quindi stampa a video il messaggio
 *  “Operazioni concluse, arrivederci dal thread: tid”, attende 5 secondi e
 *  termina.
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

int n;

void *routineProduttore(void *arg){

}

void *routineConsumatore(void *arg){

}

int main(int argc, char *argv[]){
    if(argc < 2){
        printf("Inserire un numero intero 10<=N<=15\n");
    }

    n = atoi(argv[1]);

    
}

