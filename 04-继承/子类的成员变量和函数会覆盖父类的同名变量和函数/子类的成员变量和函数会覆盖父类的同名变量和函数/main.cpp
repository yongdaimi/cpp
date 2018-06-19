#include <iostream>
#include <stdlib.h>

using namespace std;

class People {

protected:
	char *m_name;
	int m_age;

public:
	void show();

};

void People::show()
{
	cout << "hi, my name is: " << m_name << ", my age is: " << m_age << endl;
}


class Student : public People
{
private:
	float m_score;
public:
	void show();
public:
	Student(char *name, int age, float score);

};

Student::Student(char *name, int age, float score)
{
	this->m_name = name;
	this->m_age = age;
	this->m_score = score;
}

void Student::show()
{
	cout << m_name << ", my age is: " << m_age << ", my score is: " << m_score << endl;
}


int main01()
{
	Student stu("xp.chen", 16, 90.5f);
	stu.show();
	stu.People::show();

	system("pause");
	return 0;
}
















