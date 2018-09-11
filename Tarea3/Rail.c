#include "Rails.h"

char * Code(char * original, int rails)
{   
    printf("Encoding...\n");

    int len = strlen(original);

    char* result = malloc((len) * sizeof(char));

    char code[rails][len];
    
    for( int i = 0 ; i < rails ; i++){

        for( int j = 0 ; j < len ; j++){

            code[i][j]='\0';

        }
    }

    int count = 0;
    int j = 0;

    while(j<len)
    {
        if(count % 2 == 0)
        {
            for(int i = 0 ; i < rails ; i++){
                
                code[i][j]=original[j]; 
                j++;
            }   
 
        }
        else
        {   
            for(int i = rails - 2 ; i > 0 ; i--){
                
                code[i][j]=original[j];
                j++;
            }  
        } 

        count++;
    }


    int l = 0;
    
    for(int i = 0 ; i < rails; i++){

        for(int j = 0 ; j < len ; j++){

            if(code[i][j]!='\0'){

                result[l] = code[i][j];
                l++;
            }
        }
 
    }   


    return result;
}

char main(int argc, char * argv[]){
    if (argc < 3)
    {
        printf("No enough arguments recieved. Returning 0\n");
        return '\0';
    }
    else
    {
        char * original = argv[1];
        int rails = atoi(argv[2]);
        
        return * Code(original, rails);
    }
}