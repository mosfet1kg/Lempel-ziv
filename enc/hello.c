#include <stdio.h>
#include <stdlib.h>

int main(int argc, char const *argv[])
{

	char a = 0;
	char b = 255;

	if(~a == 255)
			printf("fuck");

	printf("%d\n", (char)~a);
	return 0;
}