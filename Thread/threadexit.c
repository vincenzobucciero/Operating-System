#include "apue.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <pthread.h>
#include <errno.h>

void err_exit(int err, const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    vfprintf(stderr, fmt, args);
    va_end(args);
    exit(err);
}

void *th1_fn1(void *arg) {
    printf("thread 1 returning \n");
    return ((void *) 1);
}

void *th2_fn2(void *arg) {
    printf("thread 2 exiting \n");
    return ((void *) 2);
}

int main(void) {
    pthread_t th1, th2;
    int err;
    void *tret;

    err = pthread_create(&th1, NULL, th1_fn1, NULL);
    if (err != 0)
        err_exit(err, "thread 1 creation error");
    
    err = pthread_create(&th2, NULL, th2_fn2, NULL);
    if (err != 0)
        err_exit(err, "thread 2 creation error");
    
    err = pthread_join(th1, &tret);
    if (err != 0)
        err_exit(err, "thread 1 join error");
    
    printf("thread 1 exit code %ld\n", (long int)tret);

    err = pthread_join(th2, &tret);
    if (err != 0)
        err_exit(err, "thread 1 join error");

    printf("thread 2 exit code %ld\n", (long int)tret);

    exit(0);
}