/* 
    Initial program using threads

    Cesar Valladares
    15/10/18
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define DEFAULT_THREADS 2

#define COUNTER 1000000

long total = 0;

// Mutex variable 
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

// Thread entry function
void * threadEntry (void * arg);

int main(int argc, char * argv[])
{
    int num_threads;
    
    if (argc == 2)
    {
        num_threads = atoi(argv[1]);
    }
    else
    {
        num_threads = DEFAULT_THREADS;
    }
    
    // Prepare an array for the thread id's
    pthread_t tid[num_threads];
    
    int result;
    
    // Create the number of threads specified
    for (long i=0; i<num_threads; i++)
    {
        result = pthread_create(&tid[i], NULL, threadEntry, (void*)i);
        if (result == -1)
        {
            perror("pthread_create");
            exit(EXIT_FAILURE);
        }
    }
    
    // Wait for the threads to finish
    for (int i=0; i<num_threads; i++)
    {
        pthread_join(tid[i], NULL);
        printf("Thread %d finished\n", i);
    }
    
    printf("The final total is %ld / %d\n", total, COUNTER * num_threads);
    
    //pthread_exit(NULL);
    return 0;
}

void * threadEntry (void * arg)
{
    long index = (long) arg;
    printf("Thread %ld started\n", index);
    for (int i=0; i<COUNTER; i++)
    {
        pthread_mutex_lock(&mutex);
        total++;
        pthread_mutex_unlock(&mutex);
    }
    
    pthread_exit(NULL);
}