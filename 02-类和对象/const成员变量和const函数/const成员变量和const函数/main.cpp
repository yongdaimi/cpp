#include <iostream>

using namespace std;

class Student {

private:
	char *m_name;
	int m_age;
	float m_score;

public:
	Student(char *name, int age, float score);
	void show();
	char *getName() const;
	int getAge() const;
	float getScore() const;
};


Student::Student(char *name, int age, float score)
{
	this->m_name = name;
	this->m_age = age;
	this->m_score = score;
}

void Student::show()
{
	cout << m_name << ", age is: " << m_age << "£¬score is: " << m_score << endl;
}


char * Student::getName() const
{
	return m_name;
}

int Student::getAge() const
{
	return m_age;
}

float Student::getScore() const
{
	return m_score;
}


int main()
{
	return 0;
}
