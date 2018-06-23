#include <iostream>
#include <typeinfo>
#include <stdlib.h>
using namespace std;

class People {
public:
	virtual void func();
};


void People::func(){}

class Student :public People {
};

int main()
{
	People *p;
	int n;

	cin >> n;
	if (n <= 100) {
		p = new People();
	}
	else {
		p = new Student();
	}

	if (typeid(*p) == typeid(People)) {
		cout << "I am huamn" << endl;
	}
	else {
		cout << "I am a student" << endl;
	}

	system("pause");
	return 0;
}
