/*
    César Armando Valladares
    A01023506
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <time.h>
#include <poll.h>

// Include libraries for sockets
#include <netdb.h>
#include <arpa/inet.h>

// My sockets library
#include "sockets.h"

#define SERVICE_PORT 8642
#define MAX_QUEUE 5
#define BUFFER_SIZE 1023

void usage(char * program);
void waitForConnections(int server_fd);
void communicationLoop(int connection_fd);

int main(int argc, char * argv[])
{
    int server_fd;
    
    printf("\n=== SERVER PROGRAM ===\n");

    if (argc != 2)
        usage(argv[0]);

    server_fd = startServer(argv[1]);
    
    printf("Server ready and waiting!\n");
    // Stand by to receive connections from the clients
    waitForConnections(server_fd);

    printf("Closing the server socket\n");
    // Close the socket
    close(server_fd);
    
    return 0;
}

// Show the user how to run this program
void usage(char * program)
{
    printf("Usage:\n%s {port_number}\n", program);
    exit(EXIT_FAILURE);
}



// Stand by for connections by the clients
void waitForConnections(int server_fd)
{
    struct sockaddr_in client_address;
    socklen_t client_address_size;
    char client_presentation[INET_ADDRSTRLEN];
    int connection_fd;
    pid_t pid;
    //struct pollfd test_fd[1];
    //int timeout = 1000; // Time in milliseconds (1 second)

    // Loop to wait for client connections
    while (1)
    {
        ///// ACCEPT
        // Receive incomming connections
        // Get the size of the structure where the address of the client will be stored
        client_address_size = sizeof client_address;

        while(1){

            // Fill the structure to prepare the poll 
            test_fd[0].fd = server_fd;
            test_fd[0].events = POLLIN;
            int poll_result = poll(test_fd, 1, timeout);

            if (poll_result == -1){

                perror("poll"); 
                exit(EXIT_FAILURE);
            }
            else if (poll_result == 0){
                printf("No connections yet\n");
            }
            else {
                printf("A connection is about to be accepted\n");
                break;
            }

        }

        connection_fd = accept(server_fd, (struct sockaddr *) &client_address, &client_address_size);
        if (connection_fd == -1)
        {
            perror("accept");
            exit(EXIT_FAILURE);
        }

        // Identify the client
        // Get the ip address from the structure filled by accept
        inet_ntop(client_address.sin_family, &client_address.sin_addr, client_presentation, sizeof client_presentation);
        printf("Received connection from '%s' on port '%d'\n", client_presentation, client_address.sin_port);
        
        pid = fork();
        // Parent process
        if (pid > 0)
        {
            close(connection_fd);
        }
        // Child process
        else if (pid == 0)
        {
            close(server_fd);
            // Establish the communication
            communicationLoop(connection_fd);
            exit(EXIT_SUCCESS);
        }
        // Fork error
        else
        {
            perror("fork");
            exit(EXIT_FAILURE);
        }

    }
}

int Dealer(int * house){

    int sum = 0; 
    int count = 0;

    while (sum <17){

        int card = (rand()%10)+1;
        house[count] = card;
        count++;
        sum += card;
    }

    printf("Dealer's: %d\n", sum);
    return sum;
}


// Do the actual receiving and sending of data
void communicationLoop(int connection_fd)
{
    char buffer[BUFFER_SIZE];
    //int message_counter = 0;
    
    int play = 0;
    int house[48];
    int loop = 1;

    srand(time(NULL));

    // Creating fist two cards
    int cardA = (rand()%10)+1;
    int cardB = (rand()%10)+1;

    // Variable with the total
    int sum = cardA + cardB;

    printf("Sum: %d\n", sum);
    
    // Handshake
    receiveMessage(connection_fd, buffer, BUFFER_SIZE);
    sprintf(buffer, "%d %d", cardA, cardB);
    printf("%d %d\n", cardA, cardB);
    sendMessage(connection_fd, buffer, strlen(buffer));

    while (loop != 0)
    {
        // Creating a new card
        int card = (rand()%10)+1;

        if (!receiveMessage(connection_fd, buffer, BUFFER_SIZE))
        {
            // Exit the loop when the client disconnects
            break;
        }
        sscanf(buffer, "%d", &play);
        printf("The client sent a %d\n", play);

        sum += card;
        printf("Sum: %d\n", sum);
        
        // Send card if the player wants to play and the total is under 21
        if (play == 1 && sum <= 21){

            printf("sending: %d\n", card);
            sprintf(buffer, "%d", card);

        }
        // Send "STOP" if the player wants to stop playing
        else if (play == 2){

            printf("sending STOP\n");
            sprintf(buffer, "STOP");
            break;
        }
        // Send "STOP" if the player's total is over 21
        else {

            printf("sending STOP");
            sprintf(buffer, "STOP");
            break;
        }
        
        // Send a reply
        sendMessage(connection_fd, buffer, strlen(buffer));
    }

    sendMessage(connection_fd, buffer, strlen(buffer));
    
    // Sending final result
    
    int dealernum = Dealer(house);

    // Send final result
    if (sum > 21){
        
        sprintf(buffer, "You lose with: %d", sum);

    }
    else if (dealernum > 21){

        sprintf(buffer, "You win with: %d", sum);
    }
    else if (sum > dealernum){

        sprintf(buffer, "You win with: %d", sum);
    }
    else {

        sprintf(buffer, "You lose, your hand: %d dealer's hand: %d", sum, dealernum);
    }

    sendMessage(connection_fd, buffer, strlen(buffer));

}

