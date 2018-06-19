#include <iostream>
#include <stdlib.h>
#include <iomanip>

using namespace std;


int main01()
{
	
	int a = 99;
	int &b = a;
	cout << a << ", " << b << endl;
	cout << &a << ", " << &b << endl;

	system("pause");
	return 0;
}

int num = 99;

class A {

public:
	A();
private:
	int n;
	int &r;
};


A::A() :n(0), r(num){}


int main()
{
	A *a = new A();
	cout << sizeof(A) << endl;
	cout << hex << showbase << *((int *)a + 1) << endl;
	cout << &num << endl;

	system("pause");
	return 0;
}



