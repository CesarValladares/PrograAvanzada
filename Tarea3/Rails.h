#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include <unistd.h>

char * Code(char * original, int rails);

char * Decode(char * coded, int rails);

void * Child( char* filename, int * pipe_code, char * original, int rails);

void * Parent(int * pipe_code);