#include <iostream>
#include <stdlib.h>
using namespace std;

class A {

public:
	A();
	virtual ~A();
protected:
	char *str;
};

A::A()
{
	this->str = new char[100];
	cout << "A constructor was called" << endl;
}

A::~A()
{
	delete[] str;
	cout << "A destructor was called" << endl;
}


class B : public A {

private:
	char *name;
public:
	B();
	~B();
};

B::B()
{
	this->name = new char[100];
	cout << "B constructor was called" << endl;
}

B::~B()
{
	delete name;
	cout << "B destructor was called" << endl;
}


int main()
{
	A *pa = new B();
	delete pa;

	cout << "-------------" << endl;

	B *pb = new B();
	delete pb;

	system("pause");
	return 0;
}













	










