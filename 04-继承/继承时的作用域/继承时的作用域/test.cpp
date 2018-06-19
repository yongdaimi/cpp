#include <iostream>
using namespace std;

class Base {

public:
	void func();
	void func(int);
};

void Base::func()
{
	cout << "Base::func()" << endl;
}

void Base::func(int)
{
	cout << "Base::func(int)" << endl;
}


class NewBee : public Base{
public:
	void func(char *);
	void func(bool);
};


void NewBee::func(char *)
{
	cout << "NewBee::func(char *)" << endl;
}


void NewBee::func(bool)
{
	cout << "NewBee::func(bool)" << endl;
}


int main()
{
	NewBee newBee;
	newBee.func("http://www.baidu.com");
	newBee.func(true);
	return 0;
}