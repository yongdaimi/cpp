#include <iostream>
#include <stdlib.h>
using namespace std;


class Studnet {

public:
	Studnet(char *name, int age, float score);
	void show();

private:
	static int m_total;

private:
	char *m_name;
	int m_age;
	float m_score;

public:
	static int getTotalCount();

};


int Studnet::m_total = 0;

Studnet::Studnet(char *name, int age, float score) :m_name(name), m_age(age), m_score(score)
{
	m_total++;
}

void Studnet::show(){
	cout << m_name << "的年龄是" << m_age << "，成绩是" << m_score << "（当前共有" << m_total << "名学生）" << endl;
}

int Studnet::getTotalCount()
{
	return m_total;
}


int main()
{
	(new Studnet("xp.chen", 20, 20.5f))->show();
	(new Studnet("gs.zhang", 21, 21.5f))->show();
	(new Studnet("hs.xiao", 22,22.5f))->show();
	(new Studnet("sp.sang", 23, 23.5f))->show();

	int totalCount = Studnet::getTotalCount();
	cout << "totalCount: " << totalCount << endl;

	system("pause");
	return 0;
}




