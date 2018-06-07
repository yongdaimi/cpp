#include <iostream>
#include <string>
#include <stdlib.h>
using namespace std;

class Demo {

public:
	Demo(string s);
	~Demo();

private:
	string m_s;

};

Demo::Demo(string s) :m_s(s)
{

}

Demo::~Demo(){ cout << m_s << endl; }

void func()
{
	Demo obj1("1");
}

Demo obj2("2");

int main()
{
	Demo obj3("3");
	Demo *pObj4 = new Demo("4");
	func();
	cout << "main" << endl;
	return 0;
}





