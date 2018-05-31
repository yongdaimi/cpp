#include <iostream>
#include <stdlib.h>

using namespace std;


void func(int a, int b, int c = 36);
void func(int a, int b = 5, int c);

int main()
{
	func(99);
	system("pause");
	return 0;
}

