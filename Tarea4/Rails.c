// César Armando Valladares
// A01023506

#include "Rails.h"

// Code function
char * Code(char * original, int rails)
{   

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

    // Size of original
    int msgLen = strlen(original);

    // create the matrix to cipher plain text
    char matrix[rails][msgLen];
 
    // filling the rail matrix to distinguish filled
    // spaces from blank ones
    for (int i = 0; i < rails; i++){
        for (int j = 0; j < msgLen; j++){
            matrix[i][j] = '\n';
        }
    }
 
    // to find the direction
    int direction;
 
    int row = 0;
    int col = 0;
 
    // mark the places with '*'
    for (int i = 0; i < msgLen; i++)
    {
        // check the direction of flow
        if (row == 0){
            direction = 1;
        }
        else if (row == rails-1){
            direction = 0;
        }
 
        // place the marker
        matrix[row][col++] = '*';
 
        // find the next row using direction flag

        if(direction){
            row++;
        }else{
            row--;
        }

    }
 
    // now we can construct the fill the rail matrix
    int index = 0;
    for (int i = 0; i < rails; i++){
        for (int j=0; j<msgLen; j++){
            if (matrix[i][j] == '*' && index<msgLen){
                matrix[i][j] = original[index++];
            }
        }
    }
 
 
    // now read the matrix in zig-zag manner to construct
    // the resultant text
    char  * result = malloc((msgLen +1 )* sizeof(char));
    
    int count = 0;

    row = 0, col = 0;
    for (int i=0; i< msgLen; i++)
    {
        // check the direction of flow
        if (row == 0){
            direction = 1;
        }
        else if (row == rails-1){
            direction = 0;
        }
 
        // place the marker
        if (matrix[row][col] != '*'){
            result[count] = matrix[row][col++];
            count ++;
        }
 
        // find the next row using direction flag
        if(direction){
            row++;
        }else{
            row--;
        }
    }
    return result;
}

// Child function 
void Child( char* filename, int * pipe_code, int rails, int action){

    // Clode the descriptor of reading
    close(pipe_code[0]);

    // Open the actual file open 
    FILE * pipe_file = fdopen(pipe_code[1], "w");

    char * temp;

    FILE * mainfile;

    if ((mainfile = fopen(filename, "r")) == NULL)
    { 
        perror("fopen source-file");
    } 

    char frase[500];

    // Write the coded frase
    // If coding
    if(action == 1){

        printf("coding...\n");

        // Create file
        FILE * coded = fopen("coded.txt", "w");
        
        // Code line by line
        while (fgets(frase, sizeof(frase), mainfile) != NULL)
        {
            frase[strlen(frase) - 1] = '\0'; 
            temp = Code(frase, rails);
            fprintf(coded, "%s\n", temp);
        }

        printf("Coded saved in coded.txt\n");

        fclose(coded);  
    }
    // If decoding
    else if (action == 2){
        
        printf("decoding...\n");

        // Create file
        FILE * decoded = fopen("decoded.txt", "w");

        // Decode line by line
        while (fgets(frase, sizeof(frase), mainfile) != NULL)
        {
            frase[strlen(frase) - 1] = '\0'; 
            temp = Decode(frase, rails);
            fprintf(decoded, "%s\n", temp);
        }
        printf("Coded saved in decoded.txt\n");

        fclose(decoded);

    }else{

        printf("not a valid action\n");

    }
    
    // Close
    free(temp);
    fclose(mainfile);
    fclose(pipe_file);
    close(pipe_code[1]);

}

void Parent(int * pipe_code, int action){

    FILE * pipe_file = NULL;

    if (action == 1){

        FILE * temp = fopen("coded.txt", "r");

        char buffer[500];

        // Close the descriptor for writing
        close(pipe_code[1]);

        // Open the actual file pointer
        pipe_file = fdopen(pipe_code[0], "r");


        // Get the data from the stream
        while ( fgets(buffer, 500, temp) )
        {
            printf("Coded from child: %s\n", buffer);
        }
        fclose(temp);

    }   else if (action == 2 ){

        FILE * temp = fopen("decoded.txt", "r");

        char buffer[500];

        // Close the descriptor for writing
        close(pipe_code[1]);

        // Open the actual file pointer
        pipe_file = fdopen(pipe_code[0], "r");


        // Get the data from the stream
        while ( fgets(buffer, 500, temp) )
        {
            printf("Coded from child: %s\n", buffer);
        }
        fclose(temp);
    }

    // Closes
    fclose(pipe_file);
    close(pipe_code[0]);
}

