/* 
    Comunicate processes using 'popen'

    César Valladares
    03/09/2018
*/
 #include <stdio.h>
 #include <stdlib.h>

 #define LENGTH 100

int main()
{
    //char * command = "date +%H:%M";
    char * command = "./returnArgument 367";
    //char * command = "grep 55";
    char response[LENGTH];
    
    FILE * output = popen(command, "w");
    if (!output)
    {
        perror("Could not open file");
        exit(EXIT_FAILURE);
    }
    
    while(fgets(response, LENGTH, output)){
        printf("The time I got is %s\n", response);
    }
    //fprintf(output, "2349582734h3\n");
    //fprintf(output, "987234559832\n");
    
    pclose(output);
    
    return 0;
}