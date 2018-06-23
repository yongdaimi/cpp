#include <iostream>
#include <string>
#include <stdlib.h>
using namespace std;

class People {
public:
	People(string name, int age);
public:
	virtual void display();
	virtual void eating();
protected:
	string m_name;
	int m_age;
};

People::People(string name, int age) :m_name(name), m_age(age){}
void People::display()
{
	cout << "People: " << m_name << ", age is: " << m_age << endl;
}

void People::eating()
{
	cout << "People: " << "I have eating...." << endl;
}

class Student : public People {
protected:
	float m_score;
public:
	Student(string name, int age, float score);
public:
	virtual void display();
	virtual void examing();
};

Student::Student(string name, int age, float score) :People(name, age), m_score(score){}

void Student::display()
{
	cout << "Student: " << m_name << ", age is: " << m_age << ", score is: " << m_score << endl;
}

void Student::examing()
{
	cout << "Student: " << "I am examing...." << endl;
}

class Senior : public Student {
public:
	Senior(string name, int age, float score, bool hasJob);
public:
	virtual void display();
	virtual void partying();
private:
	bool m_hasJob;
};

Senior::Senior(string name, int age, float score, bool hasJob) :Student(name, age, score), m_hasJob(hasJob){}
void Senior::display()
{
	cout << "Senior: " << m_name << ", age is: " << m_age << ", score is: " << m_score << ", hasJob is: " << m_hasJob << endl;
}

void Senior::partying()
{
	cout << "Senior: " << "I am have partying...." << endl;
}

int main()
{
	People *p = new People("xp.chen", 20);
	p->display();
	p = new Student("hy.gao", 16, 84.5f);
	p->display();
	p = new Senior("li.zhi", 22, 92.0, true);
	p->display();
	p->eating();

	system("pause");
	return 0;
}




























