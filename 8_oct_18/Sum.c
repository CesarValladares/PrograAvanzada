/*
    Program to detect when a signal has been sent and stop a process
    Uses poll to avoid blocking on certain functions to detect signals

    Gilberto Echeverria
    07/10/2018
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <poll.h>
#include <errno.h>

#define MAX_NUMBERS 20

// Global 

int interrupted = 0;

// Function

void countInputs();
void setupHandlers();
void catchInterrupt(int signal);

int main(int argc, char * argv[])
{
    setupHandlers();
    countInputs();

    return 0;
}

void countInputs()
{
    int total = 0;
    int number;

    // Variables
    struct pollfd test_fd;
    int timeout = 1000;
    int poll_result;

    // Configure the polling
    test_fd.fd = 0;     // stdin
    test_fd.events = POLLIN;

    for (int i=0; i<MAX_NUMBERS && !interrupted; i++)
    {
        printf("#%2d/%2d. Enter a number: ", i+1, MAX_NUMBERS);
        fflush(stdout);

        while(1){

            poll_result = poll(&test_fd, 1, timeout);

            // The timeout finishes without events
            if (poll_result == 0){

                printf(".");
                fflush(stdout);
                
            }
            // If there is something to read
            else if (poll_result > 0){
                
                if(test_fd.revents & POLLIN){

                    scanf("%d", &number);
                    total += number;
                    break;
                }
            }
            else { // if(poll_result == -1)
                if (errno == EINTR){
                    break;
                }
            }
        }
    }

    printf("\nThe total is: %d\n", total);
}

void setupHandlers(){
    struct sigaction new_action;

    new_action.sa_handler = catchInterrupt;
    sigfillset(&new_action.sa_mask);

    // Define signal to handle
    sigaction(SIGINT, &new_action, NULL);
}

void catchInterrupt(int signal){
    interrupted = 1;
}