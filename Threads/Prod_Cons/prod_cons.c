/*
    Program to solve the Producer-Consumer problem
    Using threads and conditionals

    Uses Escape codes to redraw the screen:
        https://en.wikipedia.org/wiki/ANSI_escape_code

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

#define NUM_SPACES 5
#define VALUE_RANGE 100
#define MAX_WAIT 5

// Global variables
int tray[NUM_SPACES];
int available_items = 0;
int read_index;
int write_index;

// Mutex and conditional variables
pthread_cond_t consumer_condition = PTHREAD_COND_INITIALIZER;
pthread_cond_t producer_condition = PTHREAD_COND_INITIALIZER;
pthread_mutex_t access_mutex = PTHREAD_MUTEX_INITIALIZER;

// Function declarations
void startThreads();
void printTray();
int randomRange(int limit);
void * producer(void * arg);
void * consumer(void * arg);

int main(int argc, char * argv[])
{
    printf("\n=== PRODUCER - CONSUMER PROGRAM ===\n");
    printf("By: Gilberto Echeverria\n");

    // Initialize the random seed
    srand((unsigned int) time(NULL));

    // Print the PID to make it easier to kill this program
    printf("Process id: %d\n", getpid());

    startThreads();

    return 0;
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

        pthread_mutex_lock(&access_mutex);

        if (available_items < NUM_SPACES){

            // Add an item to the array
            item = randomRange(VALUE_RANGE);
            index = write_index % NUM_SPACES;
            tray[index] = item;
            //printf("++ Produced item %d stored at index %d\n", item, index);
            write_index++;
            // Increase the number of products
            available_items++;

            // Let the consumer proceed
            if (available_items == 1){

                pthread_cond_signal(&consumer_condition);
            }
        }
        else {

            pthread_cond_wait(&producer_condition, &access_mutex);
        }

        pthread_mutex_unlock(&access_mutex);
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

        pthread_mutex_lock(&access_mutex);

        if (available_items > 0){

            // Read an item from the tray
            index = read_index % NUM_SPACES;
            item = tray[index];
            // Erase the contents just read
            tray[index] = 0;
            //printf("-- Consumed item %d at index %d\n", item, index);
            read_index++;
            // Reduce the number of products
            available_items--;
            // Let the producer know it can proceed
            if (available_items == NUM_SPACES -1){

                pthread_cond_signal(&producer_condition);
            }
        }
        else {

            pthread_cond_wait(&consumer_condition, &access_mutex);
        }

        pthread_mutex_unlock(&access_mutex);
    }
}
