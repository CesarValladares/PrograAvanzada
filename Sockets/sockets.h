/* 
    Library with common sockets functions
    César Valladares
    20/09/18
*/
#ifndef SOCKETS_H
#define SOCKETS_H

// Receive a message and validate the return values
int receiveMessage(int connection_fd, char * buffer, int buff_length);


// Send a message and validate
void sendMessage(int connection_fd, char * buffer, int buff_length);

int startServer(char * port);


#endif  /* not SOCKETS_H */