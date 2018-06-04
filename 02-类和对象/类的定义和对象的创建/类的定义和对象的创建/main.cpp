#include <iostream>
#include <stdlib.h>

using namespace std;

class Student {
public:
	char *name;
	int age;
	float score;

	void say() {
		cout << name << ", age is " << age << ", score is " << score << endl;
	}
};

int main1()
{
	Student stu;
	// Student allStu[100];
	stu.name = "xp.chen";
	stu.age = 15;
	stu.score = 92.5f;
	stu.say();
	system("pause");
	return 0;
}

int main()
{
	Student stu;
	// Student *pStu = &stu;
	Student *pStu = new Student;
	pStu->name = "xp.chen";
	pStu->age = 15;
	pStu->score = 92.5f;
	pStu->say();
	delete pStu;
	system("pause");
	return 0;
}

