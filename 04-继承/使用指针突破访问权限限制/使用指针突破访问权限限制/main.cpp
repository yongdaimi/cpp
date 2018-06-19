#include <iostream>
#include <stdlib.h>
using namespace std;


class A {

public:
	A(int a, int b, int c);

private:
	int m_a;
	int m_b;
	int m_c;
};


A::A(int a, int b, int c):m_a(a), m_b(b), m_c(c){}


int main()
{
	A obj(10, 20, 30);
	int a1 = *(int *)&obj;
	int b = *(int*)((int)&obj + sizeof(int));

	// int a = obj.m_a; // compile error;
	// A *p = new A(40, 50, 60);
	// int b = p->m_b; // compile error;

	cout << "a: " << a1 << ", b: " << b << endl;

	system("pause");
	return 0;
}