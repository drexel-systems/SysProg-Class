#include <stdlib.h>
#include <stdio.h>

int main(){
	//int *ptr = (int *)10;
	int *ptr = malloc(sizeof(int));
	*ptr = (int)0x1234567890ABCDEF;
	printf("ptr is at %p \n",ptr);
}
