#include <iostream>
#include <stdlib.h>
using namespace std;

template<class T> void Swap(T &a, T &b);
template<class T, unsigned int N> void Swap(T(&a)[N], T(&b)[N]);
template<typename T, unsigned int N> void printArr(T (&arr)[N]);


template<class T> void Swap(T &a, T &b)
{
	T tmp = a;
	a = b;
	b = tmp;
}

template<class T, unsigned int N> void Swap(T(&a)[N], T(&b)[N])
{
	for (int i = 0; i < N; i++) {
		T tmp = a[i];
		a[i] = b[i];
		b[i] = tmp;
	}
}

template<typename T, unsigned int N> void printArr(T(&arr)[N])
{
	for (int i = 0; i < N; i++) {
		cout << "arr[" << i << "] = " << arr[i] << endl;
	}
}

int main()
{
	int a[] = {1, 2, 3, 4, 5};
	int b[] = {6, 7, 8, 9, 10};
	Swap(a, b);
	printArr(a);
	printArr(b);
	system("pause");
	return 0;
}
























