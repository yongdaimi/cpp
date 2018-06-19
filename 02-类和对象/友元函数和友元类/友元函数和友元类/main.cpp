#include <iostream>
#include <stdlib.h>

using namespace std;

class Student {

private:
	char *m_name;
	int m_age;
	float m_score;

public:
	Student(char *name, int age, float score);

public:
	friend void show(Student *pStu);

};

Student::Student(char *name, int age, float score)
{
	this->m_name = name;
	this->m_age = age;
	this->m_score = score;
}

void show(Student *pStu)
{
	cout << pStu->m_name << ", age is: " << pStu->m_age << ", score is: " << pStu->m_score << endl;
}

int main1()
{
	Student student("xp.chen", 20, 19.5f);
	show(&student);

	Student *student1 = new Student("hy.gao", 18, 17.0f);
	show(student1);

	system("pause");

	delete(student1);
	return 0; 
}
