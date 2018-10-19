/*
    Program to solve the Producer-Consumer problem
    Using threads and semaphores

    Gilberto Echeverria
    gilecheverria@yahoo.com
    26/03/2017
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include <time.h>
#include <semaphore.h>

#define NUM_SPACES 10
#define VALUE_RANGE 100
#define MAX_WAIT 3

// Global variables
int tray[NUM_SPACES];
int read_index;
int write_index;
// Semaphore variables
sem_t available_items;
sem_t available_spaces;
sem_t access_mutex;

// Function declarations
void initSemaphores();
void startThreads();
void printTray();
int randomRange(int limit);
void * producer(void * arg);
void * consumer(void * arg);

int main(int argc, char * argv[])
{
    printf("\n=== PRODUCER - CONSUMER PROGRAM (SEMAPHORES) ===\n");
    printf("By: Gilberto Echeverria\n");

    // Initialize the random seed
    srand((unsigned int) time(NULL));

    // Print the PID to make it easier to kill this program
    printf("Process id: %d\n", getpid());

    initSemaphores();
    startThreads();

    return 0;
}

// Initialize the values of the semaphore variables
void initSemaphores()
{
    sem_init(&available_items, 0, 0);
    sem_init(&available_spaces, 0, NUM_SPACES);
    sem_init(&access_mutex, 0, 1);
}

// Prepare the two threads and initalize the data
void startThreads()
{
    pthread_t tid[2];
    int status;

    // Clear the contents of the data array
    bzero(tray, NUM_SPACES);
    // Initialize the indices
    read_index = 0;
    write_index = 0;

    // Start the two threads
    status = pthread_create(&tid[0], NULL, &producer, NULL);
    if (status)
    {
        fprintf(stderr, "ERROR: pthread_create %d\n", status);
        exit(EXIT_FAILURE);
    }
    status = pthread_create(&tid[1], NULL, &consumer, NULL);
    if (status)
    {
        fprintf(stderr, "ERROR: pthread_create %d\n", status);
        exit(EXIT_FAILURE);
    }

    // Print 3 empty lines that will be overwritten by the display of the array
    printf("\n\n\n");
    // In this main thread the contents of the array will be printed
    while (1)
    {
        printTray();
    }

    pthread_exit(NULL);
}

// Show the current contents of the array
void printTray()
{
    // Move the cursor 3 lines above
    printf("\033[3A");
    // Clear the line
    printf("\033[K");
    // Set the default color
    printf("\033[0m");
    // Print the array content
    printf("|");
    for (int i=0; i<NUM_SPACES; i++)
    {
        printf(" %3d |", tray[i]);
    }
    printf("\n");
    // Print the read and write indices
    // Clear the line
    printf("\033[K");
    // Set a green color
    printf("\033[32m");
    printf("%*cw", (write_index % NUM_SPACES) * 6 + 3, ' ');
    printf("\n");
    // Clear the line
    printf("\033[K");
    // Set a red color
    printf("\033[31m");
    printf("%*cr\n", (read_index % NUM_SPACES) * 6 + 3, ' ');
/*
    // Move the cursor 3 lines above
    printf("\033[3A");
    // Clear the line
    printf("\033[K");
    // Print the array content
    printf("|");
    for (int i=0; i<NUM_SPACES; i++)
    {
        printf(" %3d |", tray[i]);
    }
    // Print the read and write indices
    // Clear the line
    printf("\033[K");
    printf("\n%*cw", (write_index % NUM_SPACES) * 6 + 3, ' ');
    // Clear the line
    printf("\033[K");
    printf("\n%*cr\n", (read_index % NUM_SPACES) * 6 + 3, ' ');
    */
}

// Generate a random integer number between 1 and limit, inclusive
int randomRange(int limit)
{
    return rand() % limit + 1;
}

// Function to fill the array with some random item
// The time to produce is a random integer
void * producer(void * arg)
{
    int item;
    int index;

    while (1)
    {
        // Wait some time before writing another item
        sleep( randomRange(MAX_WAIT) );

        // Look for available spaces
        sem_wait(&available_spaces);
            // Lock the mutex to modify the array
            sem_wait(&access_mutex);
                // Add an item to the array
                item = randomRange(VALUE_RANGE);
                index = write_index % NUM_SPACES;
                tray[index] = item;
                //printf("++ Produced item %d stored at index %d\n", item, index);
                write_index++;
            // Unlock the mutex
            sem_post(&access_mutex);
        // Increment the number of items
        sem_post(&available_items);
    }
}

// Function to read the contents of the data array
// The time to consume is a random integer
void * consumer(void * arg)
{
    int item;
    int index;

    while (1)
    {
        // Wait some time before reading another item
        sleep( randomRange(MAX_WAIT) );

        // Look for available items
        sem_wait(&available_items);
            // Lock the mutex to modify the array
            sem_wait(&access_mutex);
                // Read an item from the tray
                index = read_index % NUM_SPACES;
                item = tray[index];
                // Erase the contents just read
                tray[index] = 0;
                //printf("-- Consumed item %d at index %d\n", item, index);
                read_index++;
            // Unlock the mutex
            sem_post(&access_mutex);
        // Increment the number of free spaces
        sem_post(&available_spaces);
    }
}
