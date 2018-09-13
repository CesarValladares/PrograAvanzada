#include "Rails.h"
#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>

int main (){

    // Getting data from user
    printf("Enter the name of the file:\n");
    char filename[50];
    scanf("%s", filename);

    printf("Enter the number of rails:\n");
    int rails = 0;
    scanf("%d", &rails);

    printf("1.- CODE    2.-DECODE\n");
    int action = 0;
    scanf("%d", &action);

    // Forking

    pid_t pid;
    pid = fork();

    // Pipe

    int pipe_code[2];
    if(pipe(pipe_code)== -1){

        // Error creating the pipe
        printf("Error!\nQuitting!\n");
        exit(EXIT_FAILURE);
    }

    if(pid == 0){

        Child(filename, pipe_code, rails, action);
        exit(0);
        
    }
    else if(pid > 0){

        wait(NULL);
        Parent(pipe_code, action);
        
    }
    else
    {
        printf("Fork failed");
    }


    return 0;
}