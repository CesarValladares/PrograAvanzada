/* 
    César Armando Valladares
    A01023506
*/

#include <stdio.h>
#include <getopt.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

#include "ppm.h"

int main(int argc, char * const argv[])
{
    int option = 0;
    char file[50];
    char targetFile[50];
    int negative = 0;
    int scale;

    while((option = getopt(argc, argv, "i:o:s:n")) != -1 ){
        switch(option){
            case 'i':
               strcpy(file, optarg);
               break;
            case 'o':
               strcpy(targetFile, optarg);
               break;
            case 'n':
                negative = 1;
                break;
            case 's':
                scale = atoi(optarg);
                break;
            default:
               break;
        }
    }

    // Create original image
    PPM* image = readPPM(file);

    // Create scaled image
    PPM* scaledImage = ScaleFile(image, scale);
    writePPM(targetFile,scaledImage);

    // Create a negative 
    if(negative == 1){
        PPM* negativeImage = InvertColor(image);
        writePPM("negative.ppm", negativeImage);
        // Free memory
        FreeMemory(negativeImage);
    }
    // Free memory
    FreeMemory(image);
    FreeMemory(scaledImage);

    return 0;
}
