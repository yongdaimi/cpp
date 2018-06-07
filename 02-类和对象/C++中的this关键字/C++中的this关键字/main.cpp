#include <iostream>
#include <stdlib.h>

using namespace std;

class Student {

private:
	char *name;
	int age;
	float score;

public:
	void setName(char *name);
	void setAge(int age);
	void setScore(float score);
	void show();

};

void Student::setName(char *name)
{
	this->name = name;
}

void Student::setAge(int age)
{
	this->age = age;
}

void Student::setScore(float score)
{
	this->score = score;
}

void Student::show()
{
	cout << this->name << ", age is: " << this->age << ", score is: " << this->score << endl;
}

int main()
{
	Student *pStu = new Student;
	pStu->setName("xp.chen");
	pStu->setAge(20);
	pStu->setScore(20.5f);
	pStu->show();
	delete pStu;

	system("pause");
	return 0;
}


