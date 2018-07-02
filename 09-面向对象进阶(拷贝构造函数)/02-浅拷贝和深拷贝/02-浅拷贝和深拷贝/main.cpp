#include <iostream>
#include <cstdlib>
using namespace std;

class Array {

private:
	int m_len;
	int *p;
public:
	Array(int len);
	Array(const Array &arr);
	~Array();
public:
	int operator[](int i) const;
	int &operator[](int i);
	Array &operator=(Array &arr);
public:
	void printArray();

};

Array::Array(int len) :m_len(len)
{
	if (len > 0) {
		p = new int[m_len];
	}
}

Array::Array(const Array &arr)
{
	this->m_len = arr.m_len;
	this->p = new int[arr.m_len];
	/*for (int i = 0; i < m_len; i++) {
		p[i] = arr[i];
	}*/
	memcpy(this->p, arr.p, m_len * sizeof(int));
}

Array::~Array()
{
	delete[] p;
}

int Array::operator[](int i) const
{
	return p[i];
}

int &Array::operator[](int i)
{
	return p[i];
}

Array &Array::operator = (Array &arr)
{
	if (this != &arr) { // 避免自己给自己赋值
		this->m_len = arr.m_len;
		delete[] p;
		this->p = new int[m_len];
		memcpy(this->p, arr.p, m_len*sizeof(int));
	}
	return *this;
}

void Array::printArray()
{
	if (m_len > 0) {
		for (int i = 0; i < m_len; i++) {
			cout << "arr[" << i << "] = " << p[i] << endl;
		}
	}
}

int main()
{
	Array arr(5);
	for (int i = 0; i < 5; i++) {
		arr[i] = i * 5 + 1;
	}
	cout << ">>>>>>>>>arr value is: <<<<<<<<<<" << endl;
	arr.printArray();

	/*Array brr = arr;
	brr[2] = 255;
	cout << ">>>>>>>>>last value is: <<<<<<<<<<" << endl;
	arr.printArray();
	brr.printArray();*/

	Array brr(5);
	for (int i = 0; i < 5; i++) {
		brr[i] = i * 2 + 1;
	}
	cout << ">>>>>>brr value is: <<<<<<<<<<<" << endl;
	brr.printArray();

	brr = arr;
	cout << ">>>>>>Last value is:<<<<<<<<<<<<"<<endl;
	arr.printArray();
	brr.printArray();

	system("pause");
	return 0;
}
































