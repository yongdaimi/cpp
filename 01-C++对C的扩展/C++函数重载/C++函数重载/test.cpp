#include <iostream>
#include <stdlib.h>

using namespace std;

void func(char ch)
{
	cout << "#1" << endl;
}

//void func(int ch)
//{
//	cout << "#2" << endl;
//}




void func(long ch)
{
	cout << "#3" << endl;
}

void func(double ch)
{
	cout << "#4" << endl;
}


int main()
{
	short s = 99;
	float f = 84.6;

	func('a'); //不需要类型转换，调用func(char)
	func(s);   //将short转换成int，调用func(int)
	func(49);  //不需要类型转换，调用func(int)
	func(f);   //将float转换成double，调用func(double)

	system("pause");
	return 0;
}
