/* 
    César Armando Valladares
    A01023506
*/

#ifndef PPM_H
#define PPM_H

//struct for pixel
typedef struct Pixel{
    unsigned char red,green,blue;
} Pixel;

//struct for ppm image
typedef struct PPM{
    int x, y;
    Pixel *data;
    int maxValue;
} PPM;

// Function to read the original file 
PPM *readPPM(const char *filename);

// Function to write a new file
void writePPM(const char *filename, PPM *image);

// Function to calculate negative image
PPM *InvertColor(PPM *image);

// Function to scale image
PPM * ScaleFile(PPM* image, int scale);

//Function to free memory
void FreeMemory(PPM* image);

#endif 