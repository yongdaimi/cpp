#include <stdio.h>
#include <stdlib.h>


int main()
{
	const int n = 10;

	int *p = (int *)&n;
	*p = 20;

	printf("n is: %d\n", n); // 在C中，输出的值是8；在C++中输出的值是仍是10
	system("pause");
	return 0;
}
