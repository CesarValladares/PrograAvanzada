#include "Rails.h"
#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>

int main (){

    char a[500];

    // Getting data from user
    
    printf("Enter the name of the file:\n");
    char filename[50] = "text.txt";
    //scanf("%s", filename);

    printf("Enter the number of rails:\n");
    int rails = 4;
    //scanf("%s", rails);

    FILE * mainFile = fopen(filename, "r");
    fgets(a, sizeof(a), mainFile);

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

        Child(filename, pipe_code, a, rails);
        exit(0);
        
    }
    else if(pid > 0){

        wait(NULL);
        Parent(pipe_code);
        
    }
    else
    {
        printf("Fork failed");
    }

    fclose(mainFile);


    return 0;
}