/* 
    Homework 1

    César Armando Valladares A01023506
    9/08/2018
*/

#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <getopt.h>

#define STRING_SIZE 50

//Type definitions
typedef union D{
    int i;
    char s[STRING_SIZE];
} Data;

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
    else if (a == 7){strcat(string, "7");}
    else if (a == 8){strcat(string, "8");}
    else if (a == 9){strcat(string, "9");}
    else if (a == 10){strcat(string, "A");}
    else if (a == 11){strcat(string, "B");}
    else if (a == 12){strcat(string, "C");}
    else if (a == 13){strcat(string, "D");}
    else if (a == 14){strcat(string, "E");}
    else if (a == 15){strcat(string, "F");}
    else if (a == 16){strcat(string, "G");}
    else if (a == 17){strcat(string, "H");}
    else if (a == 18){strcat(string, "I");}
    else if (a == 19){strcat(string, "J");}
    else if (a == 20){strcat(string, "K");}
    else if (a == 21){strcat(string, "L");}
    else if (a == 22){strcat(string, "M");}
    else if (a == 23){strcat(string, "N");}
    else if (a == 24){strcat(string, "O");}
    else if (a == 25){strcat(string, "P");}
    else if (a == 26){strcat(string, "Q");}
    else if (a == 27){strcat(string, "R");}
    else if (a == 28){strcat(string, "S");}
    else if (a == 29){strcat(string, "T");}
    else if (a == 30){strcat(string, "U");}
    else if (a == 31){strcat(string, "V");}

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
    float final = ceil(base * decimal); //final number
    
    if (aux == 0){//base case

        toChar(final,string,index);
    }
    else{//recursive 
        toChar(final,string,index);
        from10toNBase(string,aux,base,index+1);
    }

}

//function that inverts the final string
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

//function that calls all functions
void convertion(int from, int to, char *string){
    
    char number[STRING_SIZE];//original array 
    char finalNumber[STRING_SIZE];//final array
    int base1 = from; //variable for de original base
    float base2 = to; //variable for the target base 
    int numSize; //size of the original string
    float base10; // result for base 10
    int index = 0; //auxiliar index

    strcpy(number, string);

    //size of the origial number
    numSize = strlen(number);

    //array with the same size of the original number 
    int numberint[strlen(number)-1];

    //function to transform the original string to int
    trasformToInt(number, numberint, numSize);

    //base 10 result 
    base10 = toBase10(numberint, base1, numSize); 

    //transform to target base
    from10toNBase(finalNumber,base10,base2,index);
    invert_string(finalNumber);

    //prinf of the final result
    printf("Final convertion: '%s'\n", finalNumber);

    //clearing the final char
    finalNumber[0] = '\0';       
}

//function that calls the convertion 'n' times
void getoptFunction(int values, int from, int to, Data *string){

    for(int i = 0; i < values; i ++){

        printf("Converting '%s' from: %i to %i\n", string[i].s, from, to);
        convertion(from, to, string[i].s);
    }
}

int main(int argc, char **argv){

    //variables for getopt
    int option = 0;
    int from = -1;
    int to = -1;

    // flags options
    while((option = getopt(argc, argv, "i:o:")) != -1 ){
        switch(option){
            case 'i':
                from = atoi(optarg);
                break;
            case 'o':
                to = atoi(optarg);
                break;
            default:
                break;
        }
    }

    int valc = argc - optind;

    //checking if the values are correct 
    if(valc < 1 || from < 2 || to < 2 || from > 32 || to > 32){
        printf("Invalid options!\nCheck your input.\n");
        exit(1);
    }

    //valv is an array with the original characters to trasnform 
    Data* valv = malloc(valc * sizeof(Data));
    for (int index = optind; index < argc; index++){
        if(from == 10){
            valv[index - optind].i = atoi(argv[index]);
        }
        else {
            strncpy(valv[index - optind].s, argv[index], STRING_SIZE);
        }
    }

    //calling function with variables
    getoptFunction(valc,from,to, valv);

    //free memory
    free(valv);
    return 0;
}

