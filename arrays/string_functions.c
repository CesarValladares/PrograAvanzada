#include "string_functions.h"

void read_lean_string (char * string, int string_size){

    fgets(string, string_size, stdin);

    string[strlen(string)-1] = '\0';
}