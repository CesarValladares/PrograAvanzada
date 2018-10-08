/*
    Program for a simple chat client
    The server address and port are provided as arguments to the program

    Gilberto Echeverria
    gilecheverria@yahoo.com
    26/02/2017
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <netdb.h>

#include "sockets.h"

#define SERVICE_PORT 8642
#define BUFFER_SIZE 1024

void usage(char * program);
void connectToServer(char * address, char * port);
void communicationLoop(int connection_fd);

int main(int argc, char * argv[])
{
    printf("\n=== CLIENT PROGRAM ===\n");

    if (argc != 3){

        usage(argv[0]);
    }

    int play = 1;
    
    while(play == 1){

        printf("Play?\n");
        printf("Writte '1' if yes\n");
        printf("Writte '2' if no\n");
        scanf("%d", &play);

        if (play == 1){
            connectToServer(argv[1], argv[2]);
        }else{
            break;
        }

    }

    return 0;
}

// Show the user how to run this program
void usage(char * program)
{
    printf("Usage:\n%s {server_address} {port_number}\n", program);
    exit(EXIT_FAILURE);
}

// Establish a connection with the server indicated by the parameters
void connectToServer(char * address, char * port)
{
    struct addrinfo hints;
    struct addrinfo * server_info;
    int connection_fd;

    // Prepare the information to determine the local address
    bzero(&hints, sizeof hints);
    hints.ai_flags = AI_PASSIVE;
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

    ///// GETADDRINFO
    // Get the actual address of this computer
    if (getaddrinfo(address, port, &hints, &server_info) == -1)
    {
        perror("getaddrinfo");
        exit(EXIT_FAILURE);
    }

    ///// SOCKET
    // Use the information obtained by getaddrinfo
    connection_fd = socket(server_info->ai_family, server_info->ai_socktype, server_info->ai_protocol);
    if (connection_fd == -1)    
    {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    ///// CONNECT
    // Connect with the desired port
    if (connect(connection_fd, server_info->ai_addr, server_info->ai_addrlen) == -1)
    {
        perror("connect");
        exit(EXIT_FAILURE);
    }

    // Release the memory of the list of addrinfo
    freeaddrinfo(server_info);

    // Establish the communication
    communicationLoop(connection_fd);

    // Close the socket
    close(connection_fd);
}

void printCards(int * hand, int size){

    printf("Your cards:\n");

    for (int i = 0; i < size; i++){

        printf("%d ", hand[i]);

    }
    printf("\n");
}

// Do the actual receiving and sending of data
void communicationLoop(int connection_fd)
{
   
    char buffer[BUFFER_SIZE];
    int chars_read;
    int answ;

    
    // Handshake
    sprintf(buffer, "HELLO");
    sendMessage(connection_fd, buffer, strlen(buffer)); 
    receiveMessage(connection_fd, buffer, BUFFER_SIZE);


    printf("buffer %s\n", buffer);
    
    // Start game
    while ( strncmp(buffer, "STOP", 4) != 0)
    {
        printf("1.- Hit\n");
        printf("2.- Stand\n");
        scanf("%d", &answ);
        sprintf(buffer, "%d", answ);
        sendMessage(connection_fd, buffer, strlen(buffer));
    
        chars_read = receiveMessage(connection_fd, buffer, BUFFER_SIZE);
        // Finish the loop if the server disconnected
        if (!chars_read)
            break;

        if (strncmp(buffer, "STOP", 4) != 0)
        {   
            
            printf("Your new card: %s\n", buffer);
            
        }
        else if (strncmp(buffer, "STOP", 4) == 0){
           
            break;
        }
    }

    receiveMessage(connection_fd, buffer, BUFFER_SIZE);
    printf("%s\n", buffer);

}
