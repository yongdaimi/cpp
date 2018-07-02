#include <iostream>
#include <cstdlib>
using namespace std;
//复数类
class Complex{
public:
	Complex(double real = 0.0, double imag = 0.0) : m_real(real), m_imag(imag){ }
public:
	friend ostream & operator<<(ostream &out, Complex &c);  //友元函数
private:
	double m_real;  //实部
	double m_imag;  //虚部
};
//重载>>运算符
ostream & operator<<(ostream &out, Complex &c){
	out << c.m_real << " + " << c.m_imag << "i";;
	return out;
}
int main(){
	Complex a(10.0, 20.0);  //向构造函数传递 2 个实参，不使用默认参数
	Complex b(89.5);  //向构造函数传递 1 个实参，使用 1 个默认参数
	Complex c;  //不向构造函数传递实参，使用全部默认参数
	a = 25.5;  //调用转换构造函数（向构造函数传递 1 个实参，使用 1 个默认参数）

	cout << b << endl;
	system("pause");
	return 0;
}