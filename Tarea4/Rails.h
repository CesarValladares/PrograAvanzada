// César Armando Valladares
// A01023506


#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include <unistd.h>

char * Code(char * original, int rails);

char * Decode(char * coded, int rails);

void Child( char* filename, int * pipe_code, int rails, int action);

void Parent(int * pipe_code, int action);