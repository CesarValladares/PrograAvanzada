#include "Rails.h"


// Code function
char * Code(char * original, int rails)
{   
    printf("Encoding...\n");

    // Varibles thar will be used
    int msgLen = strlen(original);
    int k = -1;
    int row = 0;
    int col = 0;
    char railMatrix[rails][msgLen];

    // The coded string 
    char  * result = malloc((msgLen +1 )* sizeof(char));

    //create an empty matrix of char's
    for(int i = 0; i < rails; ++i){
        for(int j = 0; j < msgLen; ++j){
            railMatrix[i][j] = '\n';
        }
    }

    // Coding (fill the matrix in the necesary spaces)
    for(int i = 0; i < msgLen; ++i){
        railMatrix[row][col++] = original[i];
 
        if(row == 0 || row == rails-1){
            k *= -1;
        }
        row = row + k;
    }
    
    // fill the result string
    int l = 0;

    for(int i = 0; i < rails; ++i){
        for(int j = 0; j < msgLen; ++j){
            if(railMatrix[i][j] != '\n'){
                result[l] = railMatrix[i][j];
                l++;
            }
        }
    }
    return result;
}

// Decode function
char* Decode (char * original, int rails){

    // Variables that will be used
    int msgLen = strlen(enMsg);
    int k = -1; 
    int row = 0;
    int col = 0, 
    int m = 0;

    // Matrix of char's
    char railMatrix[rails][msgLen];

    // The decoded string 
    char  * result = malloc((msgLen +1 )* sizeof(char));

    // Fill matrix
    for(int i = 0; i < key; ++i){
        for(int j = 0; j < msgLen; ++j){
            railMatrix[i][j] = '\n';
        }
    }
 
    for(int i = 0; i < msgLen; ++i){
        railMatrix[row][col++] = '*';
 
        if(row == 0 || row == key-1)
            k= k * (-1);
 
        row = row + k;
    }
 
    for(int i = 0; i < key; ++i){
        for(int j = 0; j < msgLen; ++j){
            if(railMatrix[i][j] == '*'){
                railMatrix[i][j] = enMsg[m++];
            }
        }
    }
 
    // Reset variables
    row = col = 0;
    k = -1;
    int l = 0;

    // Fill final string
 
    for(int i = 0; i < msgLen; ++i){
        printf("%c", railMatrix[row][col++]);

        result[l] = railMatrix[i][j];
        l++;
 
        if(row == 0 || row == key-1)
            k= k * (-1);
 
        row = row + k;
    }
}

// Child function 
void * Child( char* filename, int * pipe_code, char * original, int rails){

    printf("Child...\n");

    // Clode the descriptor of reading
    close(pipe_code[0]);

    // Open the actual file open 
    FILE * pipe_file = fdopen(pipe_code[1], "w");

    // Open a temporal file
    FILE * temporal = fopen("Temporal.txt", "w");

    char * temp; 

    // Write the coded frase
    temp = Code(original, rails);
    printf("temp: %s\n", temp);
    
    fprintf(temporal, "%s", temp);

    // Clodse
    free(temp);
    fclose(temporal);
    fclose(pipe_file);
    close(pipe_code[1]);

}

void * Parent(int * pipe_code){

    printf("Parent...\n");

    FILE * pipe_file = NULL;

    FILE * temp = fopen("Temporal.txt", "r");

    char buffer[500];

    // Close the descriptor for writing
    close(pipe_code[1]);

    // Open the actual file pointer
    pipe_file = fdopen(pipe_code[0], "r");


    // Get the data from the stream
    printf("Reading...\n");
    while ( fgets(buffer, 500, temp) )
    {
        printf("\n[INFO] The result was saved in: %s\n", buffer);
    }

    // Closes
    fclose(temp);
    fclose(pipe_file);
    close(pipe_code[0]);
}

