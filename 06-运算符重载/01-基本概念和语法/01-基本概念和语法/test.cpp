#include <iostream>
#include <stdlib.h>
using namespace std;

class Man {

private:
	char *m_name;
	int m_age;

public:
	Man(char *name, int age);
	void display();

public:
	// Man &operator+(Man man) const;


};

Man::Man(char *name, int age) :m_name(name), m_age(age){}
void Man::display()
{	
}



//Man &Man::operator + (Man man) const
//{
//	cout << "Man1: " << this->m_name << ", Man2: " << man.m_name << endl;
//	Man *man = new Man("xxx", 13);
//	return man;
//}

int main02()
{
	/*Man man1("xp.chen", 20);
	Man man2("hy.gao", 21);

	man1 + man2;
	system("pause");*/
	return 0;
}














