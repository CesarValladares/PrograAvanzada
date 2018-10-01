/*
Example from:
http://www.bogotobogo.com/Embedded/Little_endian_big_endian_htons_htonl.php
*/

#include <iostream>
using namespace std;

int endian() {
	union {
		int one;
		char ch;
	} endn;

	endn.one = 1;
	return endn.ch;
}

int main()
{
	if(endian()) 
		cout << "little endian\n";
	else
		cout << "big endian\n";
}