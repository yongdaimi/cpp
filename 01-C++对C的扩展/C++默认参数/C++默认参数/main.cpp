#include <iostream>

using namespace std;


static void func1(int n, float b = 1.2, char c='@')
{
	cout << n << ", " << b << ", " << c << endl;
}

float d = 0.8;
static void func(int n, float b = d+2.9, char c='@')
{
	cout << n << ", " << b << ", "<<c<<endl;
}


int main1()
{
	// 为所有参数传值
	func(10, 3.5, '#');
	//为n、b传值，相当于调用func(20, 9.8, '@')
	func(20, 9.8);
	//只为n传值，相当于调用func(30, 1.2, '@')
	func(30);

	system("pause");

	return 0;
}

