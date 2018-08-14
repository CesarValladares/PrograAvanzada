/* 
    Homework 1

    César Armando Valladares A01023506
    9/08/2018
*/

#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
//#include <windows.h>

#define STRING_SIZE 50

//function to limit the array of chars
void read_lean_string (char * string, int string_size){

    fgets(string, string_size, stdin);

    string[strlen(string)-1] = '\0';
}

//function that gets a char and returns a float
float toInt(char string){

    if(string == '0'){return 0;}
    else if(string == '1'){return 1;}
    else if(string == '2'){return 2;}
    else if(string == '3'){return 3;}
    else if(string == '4'){return 4;}
    else if(string == '5'){return 5;}
    else if(string == '6'){return 6;}
    else if(string == '7'){return 7;}
    else if(string == '8'){return 8;}
    else if(string == '9'){return 9;}
    else if(string == 'A'){return 10;}
    else if(string == 'B'){return 11;}
    else if(string == 'C'){return 12;}
    else if(string == 'D'){return 13;}
    else if(string == 'E'){return 14;}
    else if(string == 'F'){return 15;}
    else if(string == 'G'){return 16;}
    else if(string == 'H'){return 17;}
    else if(string == 'I'){return 18;}
    else if(string == 'J'){return 19;}
    else if(string == 'K'){return 20;}
    else if(string == 'L'){return 21;}
    else if(string == 'M'){return 22;}
    else if(string == 'N'){return 23;}
    else if(string == 'O'){return 24;}
    else if(string == 'P'){return 25;}
    else if(string == 'Q'){return 26;}
    else if(string == 'R'){return 27;}
    else if(string == 'S'){return 28;}
    else if(string == 'T'){return 29;}
    else if(string == 'U'){return 30;}
    else if(string == 'V'){return 31;}
    else if(string == 'W'){return 32;}
    else {printf("character not valid"); return 0;}

}


//function that gets a float and returns a char
void toChar(float num, char * string, int index){

    int a = num;

    if (a == 0){strcat(string, "0");}
    else if (a == 1){strcat(string, "1");}
    else if (a == 2){strcat(string, "2");}
    else if (a == 3){strcat(string, "3");}
    else if (a == 4){strcat(string, "4");}
    else if (a == 5){strcat(string, "5");}
    else if (a == 6){strcat(string, "6");}
    else if (a == 7){string[index] = '7';}
    else if (a == 8){string[index] = '8';}
    else if (a == 9){string[index] = '9';}
    else if (a == 10){string[index] = 'A';}
    else if (a == 10){string[index] = 'A';}
    else if (a == 11){string[index] = 'B';}
    else if (a == 12){string[index] = 'C';}
    else if (a == 13){string[index] = 'D';}
    else if (a == 14){string[index] = 'E';}
    else if (a == 15){string[index] = 'F';}
    else if (a == 16){string[index] = 'G';}
    else if (a == 17){string[index] = 'H';}
    else if (a == 18){string[index] = 'I';}
    else if (a == 19){string[index] = 'J';}
    else if (a == 20){string[index] = 'K';}
    else if (a == 21){string[index] = 'L';}
    else if (a == 22){string[index] = 'M';}
    else if (a == 23){string[index] = 'N';}
    else if (a == 24){string[index] = 'O';}
    else if (a == 25){string[index] = 'P';}
    else if (a == 26){string[index] = 'Q';}
    else if (a == 27){string[index] = 'R';}
    else if (a == 28){string[index] = 'S';}
    else if (a == 29){string[index] = 'T';}
    else if (a == 30){string[index] = 'U';}
    else if (a == 31){string[index] = 'V';}

}


//function to transform into int
void trasformToInt(char * string, int * numberint, int size){

    for (int i = 0; i < size; i++){

        numberint[size - i -1] = toInt(string[i]);

    }
}

//function to transfor into base 10
float toBase10 (int * numberint, int base, int size){

    //result variable
    float res = 0; 
    
    for (int i = 0; i< size; i++){

        res += (numberint[i] * pow(base,i));

    }
    return res;
}

//function to transfrom to base 'n' from base 10
void from10toNBase(char * string, float number, float base, int index){


    float division = number/base; //result with decimals
    float aux = floor(division); //result without decimals
    float decimal = division - aux; //decimals
    float final = base * decimal; //final number

    if (aux == 0){

        toChar(number,string,index);
    }
    else{
        toChar(final,string,index);
        from10toNBase(string,aux,base,index+1);
    }

}

void invert_string(char* string){

    int size;
    string[strlen(string)] = '\0';

    size = strlen(string);

    char aux[size-1];

    for (int i =0; i< size; i++){
        aux[i] = string[i];
    }

    for (int i =0; i< size; i++){
        string[i] = aux[size-1-i];
    }
}

int convertion(){
    
    char number[STRING_SIZE];
    char finalNumber[STRING_SIZE];
    int base1;
    float base2;
    int numSize;
    float base10;
    int index = 0;

    printf("Enter the number: ");
    read_lean_string(number,STRING_SIZE);
    printf("Enter the original base: ");
    scanf("%d", &base1);
    printf("Enter the target base: ");
    scanf("%f", &base2);

    //size of the origial number
    numSize = strlen(number);

    //array with the same size of the original number 
    int numberint[strlen(number)-1];

    trasformToInt(number, numberint, numSize);

    //base 10 result 
    base10 = toBase10(numberint, base1, numSize); 

    from10toNBase(finalNumber,base10,base2,index);
    invert_string(finalNumber);

    printf("Final convertion: '%s'\n", finalNumber);

    return 0;

}

int main(){

    int a = convertion();

    return 0;
}


