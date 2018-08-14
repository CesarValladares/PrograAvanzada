/*
    Angle program 

    César Valladares

*/

#include <stdio.h>
#include <math.h>

void position (int u, int a, int t)
{
    int x = u*t*cos(a);
    int y = u*t*sin(a);

    printf ("X: '%d' Y: '%d'\n", x, y );
}

int main (){

    position(1,2,3);

    return 0;
}