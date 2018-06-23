#include <iostream>
#include <string>
#include <stdlib.h>
using namespace std;

typedef struct
{
	string name;
	int age;
	float score;
} STU;


template<class T> const T& Max(const T &a, const T &b);
template<> const STU& Max(const STU &a, const STU &b);
ostream & operator<<(ostream &out, const STU &stu);


int main01()
{
	int a = 10, b = 20;
	cout << Max(a, b) << endl;

	STU stu1 = {"xp.chen", 20, 95.5};
	STU stu2 = {"hy.gao", 19, 94.5};
	cout << Max(stu1, stu2) << endl;

	system("pause");
	return 0;
}

template<class T> const T& Max(const T &a, const T &b)
{
	return a > b ? a : b;
}

template<> const STU& Max(const STU &a, const STU &b)
{
	return a.score > b.score ? a : b;
}

ostream & operator<<(ostream &out, const STU &stu)
{
	cout << stu.name << ", " << stu.age << ", " << stu.score << endl;
	return cout;
}

































