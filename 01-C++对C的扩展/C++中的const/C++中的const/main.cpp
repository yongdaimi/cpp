#include <stdio.h>
#include <stdlib.h>


int main()
{
	const int n = 10;

	int *p = (int *)&n;
	*p = 20;

	printf("n is: %d\n", n); // ��C�У������ֵ��8����C++�������ֵ������10
	system("pause");
	return 0;
}
