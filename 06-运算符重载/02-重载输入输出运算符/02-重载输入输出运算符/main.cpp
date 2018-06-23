#include <iostream>
#include <stdlib.h>
using namespace std;

class Array {

private:
	int m_length;
	int *p; // 指向数组内存的指针
public:
	Array(int length);
	~Array();
	void display();
	int &operator[](int i);
	const int &operator[](int i) const;
};

Array::Array(int length) :m_length(length){
	if (m_length > 0) {
		p = new int[m_length ];
	}
	else {
		p = NULL;
	}
}

Array::~Array()
{
	if (p != NULL)
		delete[] p;
}

void Array::display()
{
	for (int i = 0; i < m_length; i++) {
		cout << "Array中[" << i << "] = " << p[i] << endl;
	}
}

int &Array::operator[](int i)
{
	if (p != NULL) return p[i];
}

const int &Array::operator[](int i) const
{
	if (p != NULL) return p[i];
}

int main()
{
	int n;
	cin >> n;
	Array array(n);
	for (int i = 0; i < n; i++) {
		array[i] = i * 5;
	}
	array.display();
	system("pause");
	return 0;
}























