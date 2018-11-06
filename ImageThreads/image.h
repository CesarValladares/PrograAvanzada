#include <unistd.h>
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "matrix.h"

// Assign constants to the colors
typedef enum color_enum {R, G, B} color_t;

// Structure to store a single pixel
typedef struct pixel_struct {
    // One byte: Allow max value to be 255
    unsigned char data[3];
    // Two bytes: Allow max value to be 65535
    //unsigned short data[3];
} pixel_t;

// Structure for the PPM image
typedef struct ppm_struct {
    char magic_number[3];
    int width;
    int height;
    int max_value;
    pixel_t ** pixels;
} ppm_t;

// Apply filter
void Filter(const ppm_t * input, ppm_t * output, Matrix * filter);

// Get the memory necessary to store an image of the size indicated in the structure
void allocateMemory(ppm_t * image);

// Open an image file and copy its contents to the structure
void readImageFile(char * filename, ppm_t * image);

// Write the data stored in a structure into an image file
void writeImageFile(char * filename, const ppm_t * image);

//  and store them in the arrray for the pixels
void getAsciiPixels(ppm_t * image, FILE * file_ptr);

// Copy the bytes from the image into the data array for the pixels
void getBinaryPixels(ppm_t * image, FILE * file_ptr);

// Write the numerical values for the color components into
//  the output file, giving them a nice format
void writeAsciiPixels(const ppm_t * image, FILE * file_ptr);

// Copy the pixel data into the file as bytes
void writeBinaryPixels(const ppm_t * image, FILE * file_ptr);