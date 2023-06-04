/**
 * Si realizzi un programma in C e Posix sotto Linux che, con l’ausilio della libreria Pthread, 
 * lancia 3 thread che leggono concorrentemente altrettante directory specificate dall'utente.  
 * I thread, inoltre,scrivono in  una variabile il numero di file della directory, tra le tre esaminate
 * contenente il minor numero di entrate. 
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <dirent.h>
#include <sys/stat.h>

typedef struct {
    char *directory;
    int file_count;
} threadData;

void *routine(void *arg) {
    //Passo struttura
    threadData *data = (threadData*) arg;

    //Apro directory
    DIR* dir = opendir(data->directory);
    if(dir == NULL) {
        perror("Errore apertura directory\n");
        exit(EXIT_FAILURE);
    }

    struct dirent* entry;
    int count = 0;

    while((entry = readdir(dir)) != NULL) {
        char path[1024];
        snprintf(path, sizeof(path), "%s/%s", data->directory, entry->d_name);

        struct stat file_stat;
        if (stat(path, &file_stat) == 0 && S_ISREG(file_stat.st_mode)) {
            count++;
        }
    }

    closedir(dir);
    data->file_count = count;

    pthread_exit(NULL);
}

int main(int argc, char* argv[]) {
    if (argc != 4) {
        printf("Utilizzo: %s <directory1> <directory2> <directory3>\n", argv[0]);
        return 1;
    }

    pthread_t threads[3];
    threadData thread_data[3];

    // Crea i thread per leggere le directory
    for (int i = 0; i < 3; i++) {
        thread_data[i].directory = argv[i + 1];
        thread_data[i].file_count = 0;
        pthread_create(&threads[i], NULL, routine, &thread_data[i]);
    }

    // Attendi la terminazione dei thread
    for (int i = 0; i < 3; i++) {
        pthread_join(threads[i], NULL);
    }

    // Determina la directory con il maggior numero di file
    int max_files = -1;
    char* max_directory = NULL;

    for (int i = 0; i < 3; i++) {
        printf("Directory: %s, Numero di file: %d\n", thread_data[i].directory, thread_data[i].file_count);
        if (thread_data[i].file_count > max_files) {
            max_files = thread_data[i].file_count;
            max_directory = thread_data[i].directory;
        }
    }

    printf("La directory con il maggior numero di file è: %s (%d file)\n", max_directory, max_files);

    return 0;
}