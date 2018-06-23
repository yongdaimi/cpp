#include <iostream>
using namespace std;

class complex {

private:
	double m_real;
	double m_imag;

public:
	complex();
	complex(double real, double imag);

public:
	friend complex operator+(const complex &A, const complex &B);
	void display() const;
};

complex::complex() : m_real(0.0), m_imag(0.0){}
complex::complex(double real, double imag) : m_real(real), m_imag(imag){}


complex operator+(const complex &A, const complex &B);

complex operator+(const complex &A, const complex &B)
{
	complex C;
	C.m_real = A.m_real + B.m_real;
	C.m_imag = A.m_imag + B.m_imag;
	return C;
}

void complex::display() const
{
	cout << m_real << " + " << m_imag << endl;
}

int main01()
{

	complex c1(4.3, 5.8);
	complex c2(2.4, 3.7);

	complex c3;
	c3 = c1 + c2;
	c3.display();
	return 0;
}



