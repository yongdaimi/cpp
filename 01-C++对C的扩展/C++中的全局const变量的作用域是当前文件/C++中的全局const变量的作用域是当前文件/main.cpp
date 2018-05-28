#include <stdio.h>
#include <stdlib.h>

int n = 10;
void func();

int main()
{
	func();
	printf("main: n is %d\n", n);
	system("pause");
	return 0;
}
