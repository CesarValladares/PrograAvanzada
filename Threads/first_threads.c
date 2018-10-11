/* 
    Initial program using threads

    Cesar Valladares
    11/10/18
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

void * threadEntry(void * arg);

int main(int argc, char const *argv[]){
    
    int num_threads;
    if (argc == 2){
        
        num_threads = atoi(argv[1]);
    }

    // Prepare an array for the thread id's

    pthread_t tid[num_threads];

    int result; 

    for (int i = 0; i < num_threads; i++){
        
        result = pthread_create(&tid[i], NULL, threadEntry, (void*)i);
        if (result == -1){

            perror("pthread_create");
            exit(EXIT_FAILURE);
        }
    }

    // Wait for the threads to finish
    for (int i = 0; i <num_threads; i++){
        
        long new_value;
        long * return_pointer = &new_value;
        pthread_join(tid[i], &return_pointer);
        printf("Thread %d returned with value %ld\n", i, new_value);
    }

    //pthread_exit(NULL);
    return 0;   
}

void * threadEntry(void * arg){
    
    long index = (long)arg;
    printf("This is a thread %ld, with tid %d\n", index, (int)pthread_self());
    
    pthread_exit((void *)index * 2);
}