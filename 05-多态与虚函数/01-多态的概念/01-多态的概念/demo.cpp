#include <iostream>
#include <stdlib.h>
using namespace std;


class A {

public:
	virtual void func();
	virtual void func(int);
};

void A::func()
{
	cout << "A::func()" << endl;
}

void A::func(int)
{
	cout << "A::func(int)" << endl;
}


class B : public A {
public:
	void func();
	void func(char *);
};

void B::func()
{
	cout << "B::func()" << endl;
}

void B::func(char *)
{
	cout << "B::func(char *)" << endl;
}

int main()
{
	A *p = new B();
	p->func();
	p->func(10);
	// p->func("http://www.baidu.com"); // compile error
	system("pause");
}








