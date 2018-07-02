#include <string>
#include <iostream>
#include <stdlib.h>
using namespace std;

class Student {
private:
	string m_name;
	int m_age;
	float m_score;

public:
	Student(string name = "xp.chen", int age = 20, float score = 98);
	Student(const Student &stu);
	Student &operator=(const Student &stu);

public:
	void display();
};


Student::Student(string name, int age, float score) :m_name(name), m_age(age), m_score(score){}
Student::Student(const Student &stu)
{
	this->m_name = stu.m_name;
	this->m_age = stu.m_age;
	this->m_score = stu.m_score;
	cout << "Copy constructor was called." << endl;
}
void Student::display()
{
	cout << "m_name" << ", age is: " << m_age << ", score is: " << m_score << endl;
}

Student &Student::operator = (const Student &stu)
{
	this->m_name = stu.m_name;
	this->m_age = stu.m_age;
	this->m_score = stu.m_score;
	cout << "operator= was called" << endl;
	return *this;
}


int main()
{
	Student stu1("hy.gao", 20, 100);
	Student stu2 = stu1; // call copy constructor
	Student stu3(stu1); // call copy consturctor

	stu1.display();
	stu2.display();
	stu3.display();

	system("pause");
	return 0;
}



















