#include <ctype.h>      //To use isprint
#include <stdio.h>
#include <stdlib.h>     //Use malloc and free
#include <string.h>     //To use strlen()
#include <unistd.h>     //To use opterr
#include <math.h>       //To use pow() functrion


typedef enum boolean {true,false} bool_t;

typedef struct Base {
    union{
        int decimal;
        char *non_decimal;
    };
}num_base;


void inplace_reverse(char * str){
    if (str)
    {
        char * end = str + strlen(str) - 1;
        
        // swap the values in the two given variables
        // XXX: fails when a and b refer to same memory location
#   define XOR_SWAP(a,b) do\
{\
a ^= b;\
b ^= a;\
a ^= b;\
} while (0)
        
        // walk inwards from both ends of the string,
        // swapping until we get to the middle
        while (str < end)
        {
            XOR_SWAP(*str, *end);
            str++;
            end--;
        }
#   undef XOR_SWAP
    }
}

char swithc_remainder(int num){
    char rem = '\0';
    switch (num) {
        case 10:
            rem = 'A';
            break;
        case 11:
            rem = 'B';
            break;
        case 12:
            rem = 'C';
            break;
        case 13:
            rem = 'D';
            break;
        case 14:
            rem = 'E';
            break;
        case 15:
            rem = 'F';
            break;
        case 16:
            rem = 'G';
            break;
        case 17:
            rem = 'H';
            break;
        case 18:
            rem = 'I';
            break;
        case 19:
            rem = 'J';
            break;
        case 20:
            rem = 'K';
            break;
            
        default:
            break;
    }
    return rem;
}

void convert(num_base *base,char *val, int i_val, int o_val){
    
    char * new_str = "" ;
    int lenn = (int)strlen(new_str);
    char rem = '\0';
    int bas;
    
    if (i_val == 10) {
        int remainder;
        bas = atoi(val);
        while (bas >= 1 ) {
            remainder = (int)(bas % o_val);
            if (remainder > 9) {
                rem = Snder(remainder);
            }
            else{
                rem = remainder + '0';
            }
            bas = bas / o_val;
            
            size_t len = lenn;
            char str2 = malloc(len + 1); /* one for extra char, one for trailing zero */
            str2[len] = rem;
            str2[len + 1] = '\0';
            lenn += 1;
            if (bas < 1) {
                inplace_reverse(str2);
                if (o_val == 10) {
                    base->decimal = atoi(str2);
                    printf( "Converted to base %d: %d\n\n",o_val, base->decimal );
                }
                else{
                    base->non_decimal = str2;
                    printf( "Converted to base %d: %s\n\n",o_val, base->non_decimal );
                }
            }
            free( str2 );
        }
    } else{
        
        int num = 0;
        inplace_reverse(val);
        int aux = 0;
        bool_t let = false;
        
        for (int i = 0; i < (int)strlen(val) ; i++) {
            switch (val[i]) {
                case 'A':
                    aux = 10;
                    let = true;
                    break;
                case 'B':
                    aux = 11;
                    let = true;
                    break;
                case 'C':
                    aux = 12;
                    let = true;
                    break;
                case 'D':
                    aux = 13;
                    let = true;
                    break;
                case 'E':
                    aux = 14;
                    let = true;
                    break;
                case 'F':
                    aux = 15;
                    let = true;
                    break;
                case 'G':
                    aux = 16;
                    let = true;
                    break;
                case 'H':
                    aux = 17;
                    let = true;
                    break;
                case 'I':
                    aux = 18;
                    let = true;
                    break;
                case 'J':
                    aux = 19;
                    let = true;
                    break;
                case 'K':
                    aux = 20;
                    let = true;
                    break;
                    
                default:
                    let = false;
                    break;
            }
            if (let == true) {
                num += (aux * pow(i_val, i));
            }
            else{
                char a = val[i] - '0';
                int ia = (int)a;
                num += (ia * pow(i_val,i));
            }
        }
        
        int remainder;
        while (num >= 1 ) {
            remainder = (int)(num % o_val);
            if (remainder > 9) {
                remainder = swithc_remainder(remainder);
            }
            else{
                rem = remainder + '0';
            }
            num = num / o_val;
            
            size_t len = lenn;
            char str2 = malloc(len + 1); / one for extra char, one for trailing zero */
            str2[len] = rem;
            str2[len + 1] = '\0';
            lenn += 1;
            if (num < 1) {
                inplace_reverse(str2);
                if (o_val == 10) {
                    base->decimal = atoi(str2);
                    printf( "Converted to base %d: %d\n\n",o_val, base->decimal );
                }
                else{
                    base->non_decimal = str2;
                    printf( "Converted to base %d: %s\n\n",o_val, base->non_decimal );
                }
            }
            free( str2 );
        }
        
    }
    
}




int main (int argc, char **argv){
    
    num_base num;
    
    int i_val = 0;  //Input base value
    int o_val = 0;  //Out

    int index;
    int c;
    
    opterr = 0;
    
    while ((c = getopt (argc, argv, "i:o:")) != -1)
        switch (c){
        case 'i':
                i_val = atoi(optarg);
            break;
        case 'o':
                o_val = atoi(optarg);
            break;
        case '?':
            if (optopt == 'i')
                fprintf (stderr, "Option -%c requires an argument.\n", optopt);
            else if (optopt == 'o')
                fprintf (stderr, "Option -%c requires an argument.\n", optopt);
            else if (isprint (optopt))
                fprintf (stderr, "Unknown option `-%c'.\n", optopt);
            else
                fprintf (stderr,
                         "Unknown option character `\\x%x'.\n",
                         optopt);
            return 1;
        default:
            abort ();
    }
    
    
    for (index = optind; index < argc; index++){
        printf("Converting %s from base %d to base %d\n",argv[index], i_val, o_val);
        convert(&num,argv[index],i_val,o_val);
    }
    
    return 0;
}