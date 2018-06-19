#include <iostream>
using namespace std;


int main1()
{

	int a = 99;
	int &b = a;

	b = 47;
	cout << a << ", " << b << endl;
	cout << &a << ", " << &b << endl;


	system("pause");
	return 0;
}