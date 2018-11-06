#include <unistd.h>
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "matrix.h"

//function to print any matrix
void PrintMatrix(Matrix * matrix){
    for(int i = 0; i < matrix->rows; i++){
        for(int j = 0; j < matrix->columns; j++){
            printf("%.5f ", (matrix->m)[i][j]);
        }
        printf("\n");
    } printf("\n");
}

//Reads the file and fills the matrix
Matrix * readFile(char * fileN){

    //variables 
    char data[200];
    char * num;

    //open file
    FILE* file; 

    //executes only if file exist
    if(file = fopen(fileN, "r")){

        //gets data
        fgets(data, sizeof data, file);
        
        //create matrix
        Matrix * matrix;
        matrix = (Matrix*)malloc(sizeof (Matrix));

        matrix->rows = strtol(data, &num, 10);
        matrix->columns = strtol(num, &num, 10);

        // Allocate memory
        matrix->m = (float**)malloc((matrix->rows) * sizeof(float*));
        for(int i = 0; i < matrix->rows; i++){
            matrix->m[i] = (float*)malloc((matrix->columns) * sizeof(float));
        }

        //Fill Matriz
        for (int i = 0; i < matrix->rows; i++){
            fgets(data, sizeof data, file);
            num = data;
            for(int j = 0; j < matrix->columns; j++){  
                (matrix->m)[i][j] = strtof(num, &num);
            }
        }

        //print matrix
        printf("Filter from: %s\n", fileN);
        PrintMatrix(matrix);

        //close file and return Matrix
        fclose(file);
        return matrix;
    } else {
        printf("Cannot open %s \n", fileN);
        fclose(file);
        return NULL;
    }
}

//free memory of each matrix
void freeMatrix(Matrix * m){
    for(int i = 0; i < m->columns; i++){
        free ((m->m)[i]);
    }
    free (m->m);
    free (m);
}
