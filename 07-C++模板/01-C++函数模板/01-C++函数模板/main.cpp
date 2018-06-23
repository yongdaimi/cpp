#include <iostream>
#include <stdlib.h>
using namespace std;


template<typename T> void Swap(T &a, T &b)
{
	T tmp = a;
	a = b;
	b = tmp;
}

int main()
{
	// swap int
	int n1 = 100, n2 = 200;
	Swap(n1, n2);
	cout << "n1 = " << n1 << ", n2 = " << n2 << endl;

	// swap float
	float f1 = 12.5, f2 = 52.98;
	Swap(f1, f2);
	cout << "f1 = " << f1 << ", f2 = " << f2 << endl;

	// swap char
	char c1 = 'A', c2 = 'B';
	Swap(c1, c2);
	cout << "c1 = " << c1 << ", c2 = " << c2 << endl;

	// swap bool
	bool b1 = false, b2 = true;
	Swap(b1, b2);
	cout << "b1 = " << b1 << ", b2 = " << b2 << endl;

	system("pause");
	return 0;
}

