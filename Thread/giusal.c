#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

typedef struct {
    int thread_num;
    const char *message;
} ThreadParams;

void *thread_function(void *arg) {
    ThreadParams *params = (ThreadParams *)arg;
    int thread_num = params->thread_num;
    const char *message = params->message;

    printf("Thread %d: %s\n", thread_num, message);

    // Esempio di elaborazione nel thread...

    pthread_exit(NULL);
}

int main() {
    pthread_t threads[5];
    ThreadParams params[5];
    const char *messages[5] = {
        "Hello from thread 1",
        "Hello from thread 2",
        "Hello from thread 3",
        "Hello from thread 4",
        "Hello from thread 5"
    };

    for (int i = 0; i < 5; i++) {
        params[i].thread_num = i + 1;
        params[i].message = messages[i];

        int result = pthread_create(&threads[i], NULL, thread_function, (void *)&params[i]);
        if (result != 0) {
            fprintf(stderr, "Failed to create thread %d\n", i + 1);
            exit(EXIT_FAILURE);
        }
    }

    // Attendiamo che tutti i thread terminino
    for (int i = 0; i < 5; i++) {
        pthread_join(threads[i], NULL);
    }

    return 0;
}
