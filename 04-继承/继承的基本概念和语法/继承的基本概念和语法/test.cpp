#include <iostream>
#include <stdlib.h>

using namespace std;

class People {
private:
	char *m_hobby;
protected:
	char *m_name;
	int m_age;

public:
	void setName(char *name);
	void setAge(int age);
	void setHobby(char *hobby);
	char *getHobby();
};

void People::setName(char *name)
{
	m_name = name;
}


void People::setAge(int age)
{
	m_age = age;
}

void People::setHobby(char *hobby)
{
	m_hobby = hobby;
}

char *People::getHobby()
{
	return m_hobby;
}

class Student : public People{
protected:
	float m_score;
public:
	void setScore(float score);

};

void Student::setScore(float score)
{
	m_score = score;
}

class Pupil : public Student {
private:
	int m_ranking;
public:
	void setRanking(int ranking);
	void display();
};

void Pupil::setRanking(int ranking)
{
	m_ranking = ranking;
}

void Pupil::display()
{
	cout << m_name << ", age is: " << m_age << ", score is: " << m_score << ", ranking is: " << m_ranking << ", hobby is: " << getHobby() << endl;
}

int main()
{
	Pupil pup;
	pup.setName("xp.chen");
	pup.setAge(20);
	pup.setScore(99.5f);
	pup.setRanking(4);
	pup.setHobby("PingPang BALL");
	pup.display();

	system("pause"); 
	return 0;
}










































