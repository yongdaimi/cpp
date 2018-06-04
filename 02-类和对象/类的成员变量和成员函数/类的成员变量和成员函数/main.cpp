#include <iostream>

using namespace std;

class Student {

public:
	char *name;
	int age;
	float score;

	void say();

};

void Student::say() {
	cout << name << ", age is: " << age << ", score is "<< score << endl;
}










