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

	func('a'); //����Ҫ����ת��������func(char)
	func(s);   //��shortת����int������func(int)
	func(49);  //����Ҫ����ת��������func(int)
	func(f);   //��floatת����double������func(double)

	system("pause");
	return 0;
}
