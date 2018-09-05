#include <stdio.h>
#include <stdlib.h>

int main(int argc, char * argv[])
{
    printf("This is a simple program that will return the number it gets as an argument\n");

    if (argc < 2)
    {
        printf("No arguments recieved. Returning 0\n");
        return 0;
    }
    else
    {
        int value = atoi(argv[1]);
        printf("Received a %d\n", value);
        return value;
    }
}
