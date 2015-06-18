#include <stdio.h>
#include <stdlib.h>
#include <math.h>
int calBits(unsigned n);

int main(int argc, char const *argv[])
{
	unsigned int a=0;

	printf("%u\n", a);

	printf("%d\n", calBits(1));

	printf("%c\n", 55);


	char *temp = (char*)malloc(sizeof(char)*2);

	char *tmp;
	tmp = temp;
	
	tmp = (char*)realloc(tmp, sizeof(char)*2 );



	return 0;
}

// char 

int calBits(unsigned n){
	return n==0?1:ceil( log(n+1)/log(2) );
}