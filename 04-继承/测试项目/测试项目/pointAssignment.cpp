//#include <iostream>
//#include <stdlib.h>
//using namespace std;
//
////基类A
//class A{
//public:
//	A(int a);
//public:
//	void display();
//protected:
//	int m_a;
//};
//A::A(int a) : m_a(a){ }
//void A::display(){
//	cout << "Class A: m_a=" << m_a << endl;
//}
//
////中间派生类B
//class B : public A{
//public:
//	B(int a, int b);
//public:
//	void display();
//protected:
//	int m_b;
//};
//B::B(int a, int b) : A(a), m_b(b){ }
//void B::display(){
//	cout << "Class B: m_a=" << m_a << ", m_b=" << m_b << endl;
//}
//
////基类C
//class C{
//public:
//	C(int c);
//public:
//	void display();
//protected:
//	int m_c;
//};
//C::C(int c) : m_c(c){ }
//void C::display(){
//	cout << "Class C: m_c=" << m_c << endl;
//}
//
////最终派生类D
//class D : public B, public C{
//public:
//	D(int a, int b, int c, int d);
//public:
//	void display();
//private:
//	int m_d;
//};
//
//D::D(int a, int b, int c, int d) : B(a, b), C(c), m_d(d){ }
//void D::display(){
//	cout << "Class D: m_a=" << m_a << ", m_b=" << m_b << ", m_c=" << m_c << ", m_d=" << m_d << endl;
//}
//
//int main()
//{
//	A *pa = new A(1);
//	B *pb = new B(2, 20);
//	C *pc = new C(3);
//	D *pd = new D(4, 40, 400, 4000);
//	pa = pd;
//	pa->display();
//	pb = pd;
//	pb->display();
//	pc = pd;
//	pc->display();
//	cout << "-----------------------" << endl;
//	cout << "pa=" << pa << endl;
//	cout << "pb=" << pb << endl;
//	cout << "pc=" << pc << endl;
//	cout << "pd=" << pd << endl;
//
//
//	system("pause");
//	return 0;
//}
//
//
//
//
//
//
//
//
