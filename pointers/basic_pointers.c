/*
Gilberto Echeverria

Pointers in C
*/

#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    int variable;
    int * pointer = NULL;
    double another;
    double * fptr = &another;
    
    variable = 123;
    pointer = &variable;
    
    printf("Value of 'variable' = %d\n", variable);
    printf("Address of 'variable' = %p\n", &variable);
    printf("Value of 'pointer' = %p\n", pointer);
    printf("Address of 'pointer' = %p\n", &pointer);
    printf("Value of the address pointed to by 'pointer' = %d\n", *pointer);

    // Size of the variables
    printf("Size of variable: %lu\n", sizeof variable);
    printf("Size of pointer: %lu\n", sizeof pointer);
    printf("Size of double: %lu\n", sizeof another);
    printf("Size of fptr: %lu\n", sizeof fptr);
    // Size of data types
    printf("Size of an int: %lu\n", sizeof (int));
    printf("Size of a pointer to int: %lu\n", sizeof (int*));
    printf("Size of a double: %lu\n", sizeof (double));
    printf("Size of a pointer to double: %lu\n", sizeof (double*));
    
    return 0;
}
    