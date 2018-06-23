#include <iostream>
#include <stdlib.h>
using namespace std;

class Distance {

private:
	int m_distance;

public:
	Distance(int distance);
	friend int operator-(const Distance &d1, const Distance &d2);

};

Distance::Distance(int distance) :m_distance(distance){}

int operator-(const Distance &d1, const Distance &d2);

int operator-(const Distance &d1, const Distance &d2)
{
	return d1.m_distance - d2.m_distance;
}

int main()
{
	Distance s1(5);
	Distance s2(20);

	int value = s2 - s1;
	cout << "value is: " << value << endl;

	system("pause");
	return 0;
}




