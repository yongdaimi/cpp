#include <iostream>
#include <stdlib.h>

using namespace std;


class Student {

private:
	char *m_name;
	int m_age;
	float m_score;

public:
	void setname(char *name);
	void setage(int age);
	void setscore(float score);
	void show();
};


void Student::setname(char *name)
{
	m_name = name;
}

void Student::setage(int age)
{
	m_age = age;
}

void Student::setscore(float score)
{
	m_score = score;
}

void Student::show()
{
	cout << m_name << ", age is: " << m_age << ", score is: " << m_score << endl;
}


int main()
{
	Student stu;
	cout << sizeof(stu) << endl;
	/*stu.setname("xp.chen");
	stu.setage(15);
	stu.setscore(92.5f);
	stu.show();*/

	Student *pStu = new Student;
	cout << sizeof(*pStu) << endl;
	/*pStu->setname("hy.gao");
	pStu->setage(20);
	pStu->setscore(98.0f);
	pStu->show();*/
	cout << sizeof(Student) << endl;
	delete pStu;

	system("pause");
	return 0;
}






