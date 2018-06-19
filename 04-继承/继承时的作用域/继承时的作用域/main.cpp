#include <iostream>
#include <stdlib.h>
using namespace std;

class A {

public:
	void func();
public:
	int n;
};


void A::func()
{
	cout << "http://www.baidu.com" << endl;
}

class B : public A
{
public:
	int n;
	int m;

};

class C : public B {
public:
	int n;
	int x;
};


int main01()
{
	C obj;
	obj.n;
	obj.func();
	cout << sizeof(C) << endl;

	system("pause");
	return 0;
}

