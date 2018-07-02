#include <iostream>
using namespace std;
//������
class Complex{
public:
	Complex() : m_real(0.0), m_imag(0.0){ }
	Complex(double real, double imag) : m_real(real), m_imag(imag){ }
public:
	friend ostream & operator<<(ostream &out, Complex &c);
	friend Complex operator+(const Complex &c1, const Complex &c2);
	operator double() const {return m_real}

private:
	double m_real;  //ʵ��
	double m_imag;  //�鲿
};



//����>>�����
ostream & operator<<(ostream &out, Complex &c){
	out << c.m_real << " + " << c.m_imag << "i";;
	return out;
}
//����+�����
Complex operator+(const Complex &c1, const Complex &c2){
	Complex c;
	c.m_real = c1.m_real + c2.m_real;
	c.m_imag = c1.m_imag + c2.m_imag;
	return c;
}
int main(){
	Complex c1(24.6, 100);
	double f = c1;  //�൱�� double f = Complex::operator double(&c1);
	cout << "f = " << f << endl;
	f = 12.5 + c1 + 6;  //�൱�� f = 12.5 + Complex::operator double(&c1) + 6;
	cout << "f = " << f << endl;
	int n = Complex(43.2, 9.3);  //��ת��Ϊ double����ת��Ϊ int
	cout << "n = " << n << endl;
	return 0;
}