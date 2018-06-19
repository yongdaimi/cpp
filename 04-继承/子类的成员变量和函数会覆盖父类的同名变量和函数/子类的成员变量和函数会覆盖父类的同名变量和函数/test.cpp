#include <iostream>
using namespace std;

class Parent {
public:
	void func();
	void func(int);
};

void Parent::func()
{
	cout << "Base::func()" << endl;
}

void Parent::func(int)
{
	cout << "Parent::func(int)"<<endl;
}

class Sub : public Parent {
public:
	void func(char *var);
	void func(bool var);
};

void Sub::func(char *var)
{
	cout << "Sub::func(char *)" << endl;
}

void Sub::func(bool var)
{
	cout << "Sub::func(bool)" << endl;
}


int main()
{
	Sub sub;
	sub.func("http://www.baidu.com");
	sub.func(true);
	// sub.func();
	// sub.func(10);
	sub.Parent::func();
	sub.Parent::func(100);

	return 0;
}