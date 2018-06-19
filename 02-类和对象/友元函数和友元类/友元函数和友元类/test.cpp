#include <iostream>
using namespace std;

class Address;

class Student {

private:
	char *m_name;
	int m_age;
	float m_score;

public:
	Student(char *name, int age, float score);

public:
	void show(Address *addr);
};

class Address {
private:
	char *m_province;
	char *m_city;
	char *m_district;

public:
	Address(char *province, char *city, char *district);
	friend void Student::show(Address *addr);
};

Student::Student(char *name, int age, float score)
{
	this->m_name = name;
	this->m_age = age;
	this->m_score = score;
}

void Student::show(Address *addr)
{
	cout << m_name << ", age is: " << m_age << ", score is: " << m_score << endl;
	cout << "Hotel: " << addr->m_province << " PROVINCE, " << addr->m_city << " CITY" << addr->m_district << " DISTRICT" << endl;
}









int main()
{

	return 0;
}