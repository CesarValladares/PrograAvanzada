/* 
    César Armando Valladares
    A01023506
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "ppm.h"

PPM *readPPM(const char *filename)
{

    printf("Reading image from %s...\n", filename);

    // Variables
    char buff[16];
    PPM *image;
    FILE *file;
    int PixelMaxValue;

    //open PPM file for reading
    file = fopen(filename, "rb");
    if (!file) {
        printf("Unable to open file '%s'\n", filename);
    }

    // Check the magic number
    fgets(buff, sizeof(buff), file);
    if (buff[0] != 'P' || buff[1] != '6') {
        printf("You must use a P6 image format\n");
    }

    // Alloc memory form image
    image = (PPM *)malloc(sizeof(PPM));

    // Read image size information
    fscanf(file, "%d %d", &image->x, &image->y);

    // Read rgb component
    fscanf(file, "%d", &PixelMaxValue);
    
    // Get the max value
    image->maxValue = PixelMaxValue;

    // Memory allocation for pixel data
    image->data = (Pixel*)malloc(image->x * image->y * sizeof(Pixel));


    // Read pixel data from file
    fread(image->data, 3 * image->x, image->y, file);

    fclose(file);

    return image;
}


void writePPM(const char *filename, PPM *image)
{
    printf("Writing image to %s...\n", filename);

    FILE *file;
    // Open file for output
    file = fopen(filename, "wb");

    // Magic number
    fprintf(file, "P6\n");

    // Image size
    fprintf(file, "%d %d\n",image->x,image->y);

    // Writing max value
    fprintf(file, "%d\n", image->maxValue);

    // Pixel data
    fwrite(image->data, 3 * image->x, image->y, file);
    fclose(file);
}

// Function to get negative image
PPM *InvertColor(PPM *image)
{
    printf("Creating negative image...\n");

    // Max value
    int PixelMaxValue = image->maxValue;

    // Allocate memory for the negative image 
    PPM *invertedImage;
    invertedImage = (PPM *)malloc(sizeof(PPM));
    invertedImage->maxValue = PixelMaxValue;
    invertedImage->data = (Pixel*)malloc(image->x * image->y * sizeof(Pixel));
    invertedImage->x = image->x;
    invertedImage->y = image->y;

    // Variable of the total number of pixels
    int totalSize = image->x * image->y;

    // Invert red, green and blue for each pixel
    for(int i = 0 ; i < totalSize ; i++){
        invertedImage->data[i].red = PixelMaxValue - image->data[i].red;
        invertedImage->data[i].green = PixelMaxValue - image->data[i].green;
        invertedImage->data[i].blue = PixelMaxValue - image->data[i].blue;
    }
    return invertedImage;
}

// Function to scale up or scale down 
PPM * ScaleFile(PPM* image, int scale){

    // Alloc memory for the new 
    PPM * scaledImage;
    scaledImage = (PPM* )malloc(sizeof(PPM));
    scaledImage->maxValue = image->maxValue;

    // If scaling down 
    if (scale== 50){

        printf("scaling down...\n");

        // Alloc memory for a smaller image
        scaledImage->data = (Pixel*)malloc((image->x /2)* (image->y /2) *sizeof(Pixel));
        scaledImage->x = image->x/2;
        scaledImage->y = image->y/2;

        int totalSize = scaledImage->x * scaledImage->y;

        int column = 0;
        int posOriginal;

        // Going through 
        for (int i = 0 ; i < totalSize ; i ++){

            // Calculate de position in 
            posOriginal = i * 4 - (2*column);

            // Calculate average in red
            float totalRed = image->data[posOriginal].red + image->data[posOriginal + 1].red + image->data[posOriginal + image->x].red + image->data[posOriginal + image->x + 1].red;
            float averageRed = totalRed / 4;
            // Calculate average in green 
            float totalGreen = image->data[posOriginal].green + image->data[posOriginal + 1].green + image->data[posOriginal + image->x].green + image->data[posOriginal+image->x+1].green;
            float averageGreen = totalGreen / 4;
            // Calculate average un blue
            float totalBlue = image->data[posOriginal].blue + image->data[posOriginal + 1].blue + image->data[posOriginal + image->x].blue + image->data[posOriginal+image->x+1].blue;
            float averageBlue = totalBlue / 4;

            // Fill new Pixel 
            scaledImage->data[i].red = averageRed;
            scaledImage->data[i].green = averageGreen;
            scaledImage->data[i].blue = averageBlue;

            /* As working with 1 dimensional matrix
            here I calculate the column where the 
            Pixel is*/ 
            if(((column+1)/scaledImage->x)==1){
                column = 0;
            }else{
                column++;
            }
           
        }

    }// If scaling up
    else if(scale == 200){

        printf("scaling up...\n");

        // Allocate memory for a bigger image
        scaledImage->data = (Pixel*)malloc((image->x  * 2)* (image->y * 2) *sizeof(Pixel));
        scaledImage->x = image->x*2;
        scaledImage->y = image->y*2;

        // total number of pixels of the original image
        int originalSize = image->x * image->y;

        int column = 0;
        int newPos;

        for (int i = 0 ; i < originalSize ; i ++){

            // calculate de position in the new image
            newPos = i * 4 - (2*column);

            // fill 4 pixels of the new image with the data of one original pixel
            scaledImage->data[newPos].red = image->data[i].red;
            scaledImage->data[newPos+1].red = image->data[i].red;
            scaledImage->data[newPos+scaledImage->x].red = image->data[i].red;
            scaledImage->data[newPos+scaledImage->x+1].red = image->data[i].red;

            scaledImage->data[newPos].blue = image->data[i].blue;
            scaledImage->data[newPos+1].blue = image->data[i].blue;
            scaledImage->data[newPos + scaledImage->x].blue = image->data[i].blue;
            scaledImage->data[newPos+scaledImage->x +1].blue = image->data[i].blue;

            scaledImage->data[newPos].green = image->data[i].green;
            scaledImage->data[newPos+1].green = image->data[i].green;
            scaledImage->data[newPos+scaledImage->x].green = image->data[i].green;
            scaledImage->data[newPos+scaledImage->x+1].green = image->data[i].green;

            // Calculate the column
            if(((column+1)/image->x)==1){
                column = 0;
            }else{
                column++;
            }
        }
    }

    return scaledImage;
}

// Free memory
void FreeMemory(PPM* image){
    free(image->data);
    free(image);
}