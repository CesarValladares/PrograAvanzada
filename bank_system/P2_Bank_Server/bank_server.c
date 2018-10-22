/*
    Program for a simple bank server
    It uses sockets and threads
    The server will process simple transactions on a limited number of accounts

    Gilberto Echeverria
    gilecheverria@yahoo.com
    29/03/2018

    César Armando Valladares
    A01023506
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
// Signals library
#include <errno.h>
#include <signal.h>
// Sockets libraries
#include <netdb.h>
#include <sys/poll.h>
// Posix threads library
#include <pthread.h>

// Custom libraries
#include "bank_codes.h"
#include "sockets.h"
#include "fatal_error.h"
#include <poll.h>

#define NUM_ACCOUNTS 4
#define BUFFER_SIZE 1024
#define MAX_QUEUE 5

///// Structure definitions

// Data for a single bank account
typedef struct account_struct {
    int id;
    float balance;
} account_t;

// Data for the bank operations
typedef struct bank_struct {
    // Store the total number of operations performed
    int total_transactions;
    // An array of the accounts
    account_t * account_array;
} bank_t;

// Structure for the mutexes to keep the data consistent
typedef struct locks_struct {
    // Mutex for the number of transactions variable
    pthread_mutex_t transactions_mutex;
    // Mutex array for the operations on the accounts
    pthread_mutex_t * account_mutex;
} locks_t;

// Data that will be sent to each structure
typedef struct data_struct {
    // The file descriptor for the socket
    int connection_fd;
    // A pointer to a bank data structure
    bank_t * bank_data;
    // A pointer to a locks structure
    locks_t * data_locks;
} thread_data_t;



///// FUNCTION DECLARATIONS
void usage(char * program);
void setupHandlers();
void initBank(bank_t * bank_data, locks_t * data_locks);
void waitForConnections(int server_fd, bank_t * bank_data, locks_t * data_locks);
void * attentionThread(void * arg);
int checkValidAccount(int account);
void closeBank(bank_t * bank_data, locks_t * data_locks);
/*
    EXAM: Add your function declarations here
*/

void catchInterrupt(int signal);

// Global vars
int interrupted = 0;

///// MAIN FUNCTION
int main(int argc, char * argv[])
{
    int server_fd;
    bank_t bank_data;
    locks_t data_locks;

    printf("\n=== SIMPLE BANK SERVER ===\n");

    // Check the correct arguments
    if (argc != 2)
    {
        usage(argv[0]);
    }

    // Configure the handler to catch SIGINT
    setupHandlers();

    // Initialize the data structures
    initBank(&bank_data, &data_locks);

	// Show the IPs assigned to this computer
	printLocalIPs();
    // Start the server
    server_fd = initServer(argv[1], MAX_QUEUE);
	// Listen for connections from the clients
    waitForConnections(server_fd, &bank_data, &data_locks);
    // Close the socket
    close(server_fd);

    // Clean the memory used
    closeBank(&bank_data, &data_locks);

    // Finish the main thread
    pthread_exit(NULL);

    return 0;
}

///// FUNCTION DEFINITIONS

/*
    Explanation to the user of the parameters required to run the program
*/
void usage(char * program)
{
    printf("Usage:\n");
    printf("\t%s {port_number}\n", program);
    exit(EXIT_FAILURE);
}

/*
    Modify the signal handlers for specific events
*/
void setupHandlers()
{
    signal(SIGINT, catchInterrupt);
}

void catchInterrupt(int signal){
    interrupted = 1;
}

/*
    Function to initialize all the information necessary
    This will allocate memory for the accounts, and for the mutexes
*/
void initBank(bank_t * bank_data, locks_t * data_locks)
{
    // Set the number of transactions
    bank_data->total_transactions = 0;

    // Allocate the arrays in the structures
    bank_data->account_array = malloc(NUM_ACCOUNTS * sizeof (account_t));
    // Allocate the arrays for the mutexes
    data_locks->account_mutex = malloc(NUM_ACCOUNTS * sizeof (pthread_mutex_t));

    // Initialize the mutexes, using a different method for dynamically created ones
    //data_locks->transactions_mutex = PTHREAD_MUTEX_INITIALIZER;
    pthread_mutex_init(&data_locks->transactions_mutex, NULL);
    for (int i=0; i<NUM_ACCOUNTS; i++)
    {
        //data_locks->account_mutex[i] = PTHREAD_MUTEX_INITIALIZER;
        pthread_mutex_init(&data_locks->account_mutex[i], NULL);
    }
}

/*
    Main loop to wait for incomming connections
*/
void waitForConnections(int server_fd, bank_t * bank_data, locks_t * data_locks)
{
    struct sockaddr_in client_address;
    socklen_t client_address_size;
    char client_presentation[INET_ADDRSTRLEN];
    int client_fd;
    pthread_t new_tid;
    thread_data_t * connection_data = NULL;
    int poll_response;
	int timeout = 500;		// Time in milliseconds (0.5 seconds)
    int status;

    // Get the size of the structure to store client information
    client_address_size = sizeof client_address;

    while (1)
    {
		//// POLL
        // Create a structure array to hold the file descriptors to poll
        struct pollfd test_fds[1];
        // Fill in the structure
        test_fds[0].fd = server_fd;
        test_fds[0].events = POLLIN;    // Check for incomming data
        // Check if there is any incomming communication
        poll_response = poll(test_fds, 1, timeout);

		// Error when polling
        if (poll_response == -1)
        {
            // Test if the error was caused by an interruption
            if (errno == EINTR)
            {
                printf("Poll did not finish. The program was interrupted");
            }
            else
            {
                fatalError("ERROR: poll");
            }
        }
		// Timeout finished without reading anything
        else if (poll_response == 0)
        {
            //printf("No response after %d seconds\n", timeout);
        }
		// There is something ready at the socket
        else
        {
            // Check the type of event detected
            if (test_fds[0].revents & POLLIN)
            {
				// ACCEPT
				// Wait for a client connection
				client_fd = accept(server_fd, (struct sockaddr *)&client_address, &client_address_size);
				if (client_fd == -1)
				{
					fatalError("ERROR: accept");
				}
				 
				// Get the data from the client
				inet_ntop(client_address.sin_family, &client_address.sin_addr, client_presentation, sizeof client_presentation);
				printf("Received incomming connection from %s on port %d\n", client_presentation, client_address.sin_port);

				// Prepare the structure to send to the thread
                
                connection_data = malloc(sizeof(thread_data_t));
                connection_data->bank_data = bank_data;
                connection_data->data_locks = data_locks;
                connection_data->connection_fd = client_fd;

				// CREATE A THREAD
                status = pthread_create(&new_tid, NULL, attentionThread, (void*)connection_data);

                if (status != 0){
                    
                    fprintf(stderr, "ERROR: pthread_create %d\n", status);
                    exit(EXIT_FAILURE);

                }

            }
        }
    }

    // Print the total number of transactions performed
    pthread_mutex_lock(&connection_data->data_locks->transactions_mutex);
    int num_trans = connection_data->bank_data->total_transactions;
    printf("Total transactions: %d\n", num_trans);
    pthread_mutex_unlock(&connection_data->data_locks->transactions_mutex);
}

/*
    Hear the request from the client and send an answer
*/
void * attentionThread(void * arg)
{
    // Receive the data for the bank, mutexes and socket file descriptor
    thread_data_t * thread_data = (thread_data_t*)arg;

    //varaibles dor poll the standard input
    struct pollfd test_fd;
    int timeout = 1000;
    int poll_result;
    char buffer[BUFFER_SIZE];

    int chars_read;

    test_fd.fd = thread_data->connection_fd;
    test_fd.events = POLLIN;


    // Loop to listen for messages from the client
    while(!interrupted){
        

        poll_result = poll(&test_fd, 1, timeout);
        // Receive the request

        if (poll_result == 0){

        }
        else if (poll_result > 0){
            
            printf("Reciving...\n");
            chars_read = recvString(test_fd.fd, buffer, BUFFER_SIZE);
            if (chars_read == -1){
                fatalError("recv");
            }

            int operation;
            int account;
            float amount;

            sscanf(buffer, "%d %d %f", &operation, &account, &amount);

            account_t* user = &thread_data->bank_data->account_array[account];
            pthread_mutex_t alock = thread_data->data_locks->account_mutex[account];
            pthread_mutex_t translock = thread_data->data_locks->transactions_mutex;

            if (!checkValidAccount(account)){

                sprintf(buffer, "%d %f", NO_ACCOUNT, 0.0);
            }
            else if (operation == EXIT){
                break;
            }
            else if (amount < 0){

                sprintf(buffer, "%d %f", ERROR, 0.0);
                sendString(thread_data->connection_fd, buffer);

            }
            else if (operation == CHECK){

                pthread_mutex_lock(&alock);
                    pthread_mutex_lock(&translock);
                        amount = user->balance;
                        thread_data->bank_data->total_transactions++;
                    pthread_mutex_unlock(&translock);
                pthread_mutex_unlock(&alock);

            }
            else if (operation == DEPOSIT){

                pthread_mutex_lock(&alock);
                    pthread_mutex_lock(&translock);
                        user->balance += amount;
                        amount = user->balance;
                        thread_data->bank_data->total_transactions++;
                    pthread_mutex_unlock(&translock);
                pthread_mutex_unlock(&alock);                
                
            }
            else if (operation == WITHDRAW){

                pthread_mutex_lock(&alock);
                    pthread_mutex_lock(&translock);
                        user->balance -= amount;
                        int new_amount = user->balance;

                        if (new_amount < 0 ){

                            user->balance += amount;                            
                            sprintf(buffer, "%d %f", INSUFFICIENT, 0.0);
                            sendString(thread_data->connection_fd,buffer);


                        }else{

                            thread_data->bank_data->total_transactions++;
                            amount = new_amount;
                        }
                    pthread_mutex_unlock(&translock);
                pthread_mutex_unlock(&alock);  

                
            }
            else{

                sprintf(buffer, "%d %f", ERROR, 0.0);
                sendString(thread_data->connection_fd, buffer);

            }

            sprintf(buffer, "%d %f", OK, amount);
            sendString(thread_data->connection_fd, buffer);

        }

        // Process the request being careful of data consistency

        // Update the number of transactions

        // Send a reply
    }

    sprintf(buffer, "%i %d",  BYE, 0);
    sendString(thread_data->connection_fd, buffer);
    free(thread_data);
    pthread_exit(NULL);
}

/*
    Free all the memory used for the bank data
*/
void closeBank(bank_t * bank_data, locks_t * data_locks)
{
    printf("DEBUG: Clearing the memory for the thread\n");
    free(bank_data->account_array);
    free(data_locks->account_mutex);
}

/*
    Return true if the account provided is within the valid range,
    return false otherwise
*/
int checkValidAccount(int account)
{
    return (account >= 0 && account < NUM_ACCOUNTS);
}
