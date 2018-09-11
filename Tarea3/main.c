#include "Rails.h"
#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>

int main (){

    char a[500];

    FILE * file;

    // Getting data from user
    
    printf("Enter the name of the file:\n");
    char filename[50];
    scanf("%s", filename);

    printf("Enter the number of rails:\n");
    char rails[10];
    scanf("%s", rails);

    // Forking

    pid_t pid;
    pid = fork();

    if(pid > 0){

        int status;

        printf("Father\n");

        wait(&status);

        printf("Father2\n");
        printf("My child returned a %d\n", WEXITSTATUS(status));
    }
    else if(pid == 0){

        printf("child\n");

        file = fopen(filename, "r");

        while (fgets(a, sizeof(a), file)) {

            printf("Reading...\n");

            a[strlen(a) - 1] = '\0';
            
            char * program = "Rail";
            char * arguments[] = {"Rail", a, rails,(char*)NULL};
            
            printf("Data\n");
            execv(program, arguments);
            
        }
        fclose(file);
    }
    else
    {
        printf("Fork failed");
    }

    return 0;
}