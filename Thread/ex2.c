#include "apue.h"
#include <pthread.h>

//struttura per passaggio di parametri per char_print
struct char_print_params {
    char c;
    int n;
};

void* char_print(void* parameter) {
    //cast del puntatore al tipo corretto
    struct char_print_params* p = (struct char_print_params*) parameter;
    int i;

    for(i=0; i<p->n; ++i)
    printf("%c", p->c);
        fputc(p->c, stderr);
    return NULL;
}

int main() {
    pthread_t tid1;
    pthread_t tid2;
    struct char_print_params tid1_args;
    struct char_print_params tid2_args;

    //creo thread per stampare 100 'x'
    tid1_args.c = 'x';
    tid1_args.n = 100;
    pthread_create(&tid1, NULL, char_print, (void *) &tid1_args);

    //creo thread per stampare 100 'y'
    tid2_args.c = 'y';
    tid2_args.n = 100;
    pthread_create(&tid2, NULL, char_print, (void *) &tid2_args);

    sleep(1);

    return 0;
}