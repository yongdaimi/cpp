#include <iostream>
#include <stdlib.h>
using namespace std;


template<typename T> void Swap(T &a, T &b)
{
	T tmp;
	tmp = a;
	a = b;
	b = tmp;
}


template<class T> void Swap(T *a, T *b, int len)
{
	for (int i = 0; i < len; i++) {
		T tmp = a[i];
		a[i] = b[i];
		b[i] = tmp;
	}
}


template<class T> void printArray(T *pArray, int len)
{
	for (int i = 0; i < len; i++) {
		cout << "p[" << i << "] = " << pArray[i] << endl;
	}
}


int main()
{
	int n1 = 10, n2 = 20;
	Swap(n1, n2);

	cout << "n1 = " << n1 << ", n2 = " << n2 << endl;
	cout << "-----------------------" << endl;
	int a[] = {1, 2, 3, 4, 5};
	int b[] = {6, 7, 8, 9, 10};
	int len = sizeof(a) / sizeof(int);
	Swap(a, b, len);
	
	cout << "a[] is: " << endl;
	printArray(a, len);

	cout << "b[] is: " << endl;
	printArray(b, len);

	system("pause");
	return 0;
}






