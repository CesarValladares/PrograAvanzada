/* 
    César Armando Valladares 
    A01023506 

    24/08/2018

    to use after compile it do:

    ./executbale "file name 1" "file name 2"
*/

#include <stdio.h>
#include <stdlib.h>

// Matrix struct

typedef struct M{
    int rows; 
    int columns; 
    float ** m;
} Matrix;

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
        printf("Matrix from: %s\n", fileN);
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

//Writes the result matrix in a file 
void WriteMatrix(Matrix * matrix){
    FILE* file = fopen("finalMatrix.txt", "w");

    fprintf(file, "%i %i\n", matrix->rows, matrix->columns);
    for(int i = 0; i < matrix->rows; i++){
        for(int j = 0; j < matrix->columns; j++){
            fprintf(file, "%f ", (matrix->m)[i][j]);
        }
        fprintf(file, "\n");
    }
    fclose(file);
}

//Biggest function where 2 matrices are created and multiplied 
void Mult(char * file1, char * file2){


    //Read 2 files and create 2 matrices
    Matrix * m1 = readFile(file1);
    Matrix * m2 = readFile(file2);
    
    //only if the matrices 
    if(m1->columns == m2->rows){

        //Create result matrix
        Matrix * result;
        result = (Matrix*)malloc(sizeof (Matrix));

        result->rows = m1->rows;
        result->columns = m2->columns;

        // Allocate memory
        result->m = (float**)malloc((result->rows) * sizeof(float*));
        for(int i = 0; i < result->rows; i++){
            result->m[i] = (float*)malloc((result->columns) * sizeof(float));
        }
        
        // Multiply and fill the result matrix
        for(int i = 0; i < m1->rows; i++){
            for(int j = 0; j < m2->columns; j++){
                float sum = 0;
                for(int k = 0; k < m1->columns; k++){

                    sum += (m1->m)[i][k] * (m2->m)[k][j];
                }
                (result->m)[i][j] = sum;
            }
        }
        
        //print matrix and write in a file
        printf("Result Matrix: \n");
        PrintMatrix(result);
        printf("Writting new txt file\n");
        WriteMatrix(result);

    }else{
        //If the matrices cannot be multiplied
        printf("!!!CANNOT MULTIPLY MATRICES!!!\n");
    }
}

int main(int argc, char *argv[]){
    
    Mult(argv[1], argv[2]);
    
    return 0;
}