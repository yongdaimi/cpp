#include <iostream>
#include <stdlib.h>

using namespace std;

class People {
protected:
	char *m_name;
	int m_age;
public:
	People(char *, int);
};

People::People(char *name, int age) :m_name(name), m_age(age){}


class Student : public People{
private:
	float m_score;
public:
	Student(char *name, int age, float score);
	void display();
};

Student::Student(char *name, int age, float score) :People(name, age), m_score(score){}

Student::Student(char *name, int age, float score){
	People(name, age);
	this->m_score = score;
}


void Student::display()
{
	cout << m_name << ", age is: " << m_age << ", score is: " << m_score << endl;
}

int main()
{
	Student stu("xp.chen", 16, 90.5f);
	stu.display();

	system("pause");
	return 0;
}



