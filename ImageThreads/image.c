
#include "image.h"
#include <math.h>

// Get the memory necessary to store an image of the size indicated in the structure
void allocateMemory(ppm_t * image)
{
    // Allocate the memory for INDEX array
    image->pixels = malloc (image->height * sizeof(pixel_t *));
    // Allocate the memory for all the DATA array
    image->pixels[0] = calloc (image->height * image->width, sizeof(pixel_t));
    
    // Add the rest of the pointers to the INDEX array
    for (int i=1; i<image->height; i++)
    {
        // Add an offset from the beginning of the DATA array
        image->pixels[i] = image->pixels[0] + image->width * i;
    }
}

// Apply filter
void Filter(const ppm_t * input, ppm_t * output, Matrix * filter){

    int ref = floor(filter->columns/2);
    int cols = input->height;
    int rows = input->width;

    allocateMemory(output);

    long i, ii, j, jj;

    for (i = 0; i < cols; i ++){
        for (j = 0; j < rows ; j++){

            // Variables for the pixel values 
            int blue = 0;
            int green = 0;
            int red = 0;

            int contador = 0;

            for (ii = -ref ; ii <= ref; ii++){
                for (jj = -ref; jj <= ref;  jj++){

                    if (i + ii > 0 && j + jj > 0 && j + jj < rows && i + ii < cols){

                        int x = ii + ref;
                        int y = jj + ref;
                        contador += filter->m[x][y];

                        //printf("xsi: %d, y: %d, m: %f\n", x,y,filter->m[x][y]);

                        blue += (input->pixels[i+ii][j+jj].data[0] * filter->m[x][y]);
                        green += (input->pixels[i+ii][j+jj].data[1] * filter->m[x][y]);
                        red += (input->pixels[i+ii][j+jj].data[2] * filter->m[x][y]);
                    }
                    else {
                        
                       
                        //printf("xno: %d, y: %d, m: %f\n", x,y,filter->m[x][y]);
                    }

                }
            } 

            if (contador == 0){
                
                output->pixels[i][j].data[0] = (blue);
                output->pixels[i][j].data[1] = (green);
                output->pixels[i][j].data[2] = (red );
                
            }else {
                //printf("Contador: %d\n", contador);
                output->pixels[i][j].data[0] = (blue/contador);
                output->pixels[i][j].data[1] = (green/contador);
                output->pixels[i][j].data[2] = (red/contador);

            }

            

        }
    }

}

// Open an image file and copy its contents to the structure
void readImageFile(char * filename, ppm_t * image)
{
    FILE * file_ptr = NULL;
    
    file_ptr = fopen (filename, "r");
    if (!file_ptr)
    {
        printf("Unable to open the file '%s'\n", filename);
        exit(EXIT_FAILURE);
    }
    
    // Get the data from the header
    fscanf (file_ptr, "%s", image->magic_number);
    fscanf (file_ptr, "%d", &image->width);
    fscanf (file_ptr, "%d", &image->height);
    fscanf (file_ptr, "%d", &image->max_value);
    // Get rid of the '\n' after max value and before the real data
    fgetc(file_ptr);
    
    // Get the memory for the image data
    allocateMemory(image);
    
    if (!strncmp(image->magic_number, "P3", 3))
    {
        getAsciiPixels(image, file_ptr);
    }
    else if (!strncmp(image->magic_number, "P6", 3))
    {
        getBinaryPixels(image, file_ptr);
    }
    else
    {
        printf("Not a correct PPM format");
    }

    fclose(file_ptr);
}

// Write the data stored in a structure into an image file
void writeImageFile(char * filename, const ppm_t * image)
{
    FILE * file_ptr = NULL;
    
    file_ptr = fopen (filename, "w");
    if (!file_ptr)
    {
        printf("Unable to open the file '%s'\n", filename);
        exit(EXIT_FAILURE);
    }
    
    // Insert the header
    fprintf (file_ptr, "%s\n", image->magic_number);
    fprintf (file_ptr, "%d ", image->width);
    fprintf (file_ptr, "%d\n", image->height);
    fprintf (file_ptr, "%d\n", image->max_value);

    // Write the matrix of data for the pixels
    if (!strncmp(image->magic_number, "P3", 3))
    {
        writeAsciiPixels(image, file_ptr);
    }
    else if (!strncmp(image->magic_number, "P6", 3))
    {
        writeBinaryPixels(image, file_ptr);
    }
    else
    {
        printf("Not a correct PPM format");
    }

    fclose(file_ptr);
}


// Read each of the numerical values for the color components
//  and store them in the arrray for the pixels
void getAsciiPixels(ppm_t * image, FILE * file_ptr)
{
    for (int r=0; r<image->height; r++)
    {
        for (int c=0; c<image->width; c++)
        {
            fscanf (file_ptr, "%hhu", &image->pixels[r][c].data[R]);
            fscanf (file_ptr, "%hhu", &image->pixels[r][c].data[G]);
            fscanf (file_ptr, "%hhu", &image->pixels[r][c].data[B]);
            //fscanf (file_ptr, "%hu", &image->pixels[r][c].data[B]);
        }
    }
}

// Copy the bytes from the image into the data array for the pixels
void getBinaryPixels(ppm_t * image, FILE * file_ptr)
{
    fread (image->pixels[0], sizeof(pixel_t), image->width * image->height, file_ptr);    
}

// Write the numerical values for the color components into
//  the output file, giving them a nice format
void writeAsciiPixels(const ppm_t * image, FILE * file_ptr)
{
    for (int r=0; r<image->height; r++)
    {
        for (int c=0; c<image->width; c++)
        {
            fprintf (file_ptr, "%3hhu ", image->pixels[r][c].data[R]);
            fprintf (file_ptr, "%3hhu ", image->pixels[r][c].data[G]);
            fprintf (file_ptr, "%3hhu\t", image->pixels[r][c].data[B]);
        }
        fprintf(file_ptr, "\n");
    }
}

// Copy the pixel data into the file as bytes
void writeBinaryPixels(const ppm_t * image, FILE * file_ptr)
{
    fwrite (image->pixels[0], sizeof(pixel_t), image->width * image->height, file_ptr);    
}
