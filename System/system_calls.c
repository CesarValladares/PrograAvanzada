
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{
   printf("In the main program: \n");
   execl("/bin/ls", "ls", "-l","..", (char*)NULL);
   execvp();

   printf("Returning to my main\n");

   return 0;
}