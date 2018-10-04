#include<stdio.h>
#include<string.h>
#include<stdlib.h>

int main()
{
    int rails;
    
    int count= 0;
    
    char code[100][1000];

    char str[1000];

    printf("Enter a Secret Message\n");
    scanf("%[^\n]s", str);

    int len=strlen(str);

    printf("Enter number of rails\n");
    scanf("%d", &rails);


    for(int i = 0 ; i < rails ; i++){
        for( int j = 0 ; j < len ; j++){

            code[i][j]=0;
        }
    }

    int j = 0;

    while( j <len)
    {
        if(count%2==0)
        {
            for(int i = 0 ; i <rails ; i++){
                
                code[i][j]=str[j]; 

                j++;
            }   
 
        }
        else
        {   
            for(int i = rails - 2 ; i > 0 ; i--){
                code[i][j]=str[j];
                j++;
            }  
        } 

        count++;
    }

    for( int i = 0 ; i < rails ; i++){
        for(j = 0 ; j < len ; j++){

            if(code[i][j]!=0)
                printf("%c",code[i][j]);
        }
 
    }printf("\n");

    return 0;
}