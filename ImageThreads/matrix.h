
// Matrix struct
typedef struct M{
    int rows; 
    int columns; 
    float ** m;
} Matrix;

//function to print any matrix
void PrintMatrix(Matrix * matrix);

//Reads the file and fills the matrix
Matrix * readFile(char * fileN);

//free memory of each matrix
void freeMatrix(Matrix * m);
