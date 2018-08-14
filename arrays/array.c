/*
    experiments with strings

    Cesar Valladares
    09/08/18
    */

#include <stdio.h>
#include <string.h>
#include "string_functions.h"

#define STRING_SIZE 50

int main(){

    char name[STRING_SIZE];

    printf("'%s'\n", name);

    //name[5] = '\0';
    //strncpy(name, "Tom Sawyer", STRING_SIZE);
    //sprintf(name, "123 %s 456", name);

    printf ("Enter your name: ");
    //scanf("%s", name);

    //printf("name: '%s'\n", name);

    //fgets(name, STRING_SIZE, stdin);

    //name[strlen(name)-1] = '\0';

    read_lean_string(name, STRING_SIZE);

    printf("name: '%s'\n", name);

    return 0;

}