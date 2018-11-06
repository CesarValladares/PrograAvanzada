
// Matrix struct
typedef struct M{
    int rows; 
    int columns; 
    int ** m;
} Matrix;

//function to print any matrix
void PrintMatrix(Matrix * matrix);

//Reads the file and fills the matrix
Matrix * readFile(char * fileN);

//free memory of each matrix
void freeMatrix(Matrix * m);
