#include "image.h"

void applyFilter(char * argv[]){

    // Get the names of the files
    char * input = argv[1];
    char * output = argv[2];
    char * matrixFile = argv[3];

    // Reads the filter matrix
    Matrix * filter = readFile(matrixFile);

    // Create the image variables and initialize to be empty
    ppm_t input_image = {"", 0, 0, 0, NULL};
    ppm_t output_image = {"", 0, 0, 0, NULL};

    // Load the image into the program
    readImageFile(input, &input_image);
    printf("Read file '%s' successfully\n", input);
    printf("Image size: %d %d\n", input_image.height, input_image.width);

    strncpy (output_image.magic_number, input_image.magic_number, 3);
    output_image.max_value = input_image.max_value;
    output_image.height = input_image.width;
    output_image.width = input_image.height;

    Filter(&input_image, &output_image, filter);

    writeImageFile(output, &output_image);

}

int main(int argc, char * argv [])
{
    if(argc < 4){

        printf("Not enough arguments\n");

    }else {

        applyFilter(argv);

    }

    return 0;
}