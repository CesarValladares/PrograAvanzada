#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[])
{
    pid_t pid;
    char number[10]; 

    int iterations = 30;
    if (argc == 2){
        iterations = atoi(argv[1]);
    }

    printf("In the main program with PID: %d\n", getpid());

    for (int i = 0; i < iterations; i++){
    
        pid = fork();
        
        if (pid > 0)    // The parent process
        {
            int status;
            //printf("PARENT PID: %d, with parent %d\n", getpid(), getppid());
            //printf("The parent received pid: %d\n", pid);
            wait(&status);
            printf("My child returned with %x\n", status);
            if (WIFEXITED(status))
            {
                printf("My child returned a %d\n", WEXITSTATUS(status));
            }
        }
        else if (pid == 0)      // The child process 
        {
            //printf("\tCHILD PID: %d, with parent %d\n", getpid(), getppid());
            //printf("\tFork returned %d in the child\n", pid);
            char * program = "returnArgument";
            sprintf(number, "%d", i);
            char * arguments[] = {"returnArgument", number, (char*) NULL};
            execv(program, arguments);
            //exit(EXIT_FAILURE);
            //return 10;
        }
        else
        {
            printf("Fork failed");
        }
    }
    
    //nothing = getchar();
   
    printf("Returning to my main\n");
    
    return 0;
}