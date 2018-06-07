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
	void show();
};


//Student::Student(char *name, int age, float score)
//{
//	m_name = name;
//	m_age = age;
//	m_score = score;
//}


Student::Student(char *name, int age, float score) :m_name(name), m_age(age), m_score(score)
{
	
}



void Student::show()
{
	cout << m_name << ", age is: " << m_age << ", score is: " << m_score << endl;
}


int main1()
{
	Student stu("xp.chen", 15, 92.5f);
	stu.show();
	
	Student *pStu = new Student("xp.chen", 16, 96);
	pStu->show();
	delete pStu;

	system("pause");
	return 0;
}