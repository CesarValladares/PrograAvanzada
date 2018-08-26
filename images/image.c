#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void readpmm(){

    FILE *file;
	char type[3];
    float matrixSize[2];
    float max;
	
	file = fopen("tester-a.ppm","r");
	
	if (file == NULL)
    {
        printf("\nError \n\n");
    }
    else
    {
        /*printf("\nContent \n\n");
        
        while((character = fgetc(file)) != EOF)
        {
        printf("%c",character);
        }*/

        fgets(type,3,file);
        printf("%s\n",type);

        fscanf(file, "%f %f", &matrixSize[0], &matrixSize[1]);

        printf("%.1f\n",matrixSize[0]);
        printf("%.1f\n",matrixSize[1]);

        fscanf(file, "%f", &max);

        printf("%.1f\n",max);

                
    }
    
    fclose(file);
}

int main (){

    readpmm();
    return 0;
}