#include <iostream>
#include <cstdlib>
using namespace std;
//������
class Complex{
public:
	Complex(double real = 0.0, double imag = 0.0) : m_real(real), m_imag(imag){ }
public:
	friend ostream & operator<<(ostream &out, Complex &c);  //��Ԫ����
private:
	double m_real;  //ʵ��
	double m_imag;  //�鲿
};
//����>>�����
ostream & operator<<(ostream &out, Complex &c){
	out << c.m_real << " + " << c.m_imag << "i";;
	return out;
}
int main(){
	Complex a(10.0, 20.0);  //���캯������ 2 ��ʵ�Σ���ʹ��Ĭ�ϲ���
	Complex b(89.5);  //���캯������ 1 ��ʵ�Σ�ʹ�� 1 ��Ĭ�ϲ���
	Complex c;  //�����캯������ʵ�Σ�ʹ��ȫ��Ĭ�ϲ���
	a = 25.5;  //����ת�����캯�������캯������ 1 ��ʵ�Σ�ʹ�� 1 ��Ĭ�ϲ�����

	cout << b << endl;
	system("pause");
	return 0;
}