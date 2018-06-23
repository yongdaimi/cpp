//#include <iostream>
//#include <stdlib.h>
//using namespace std;
//
//class People {
//
//protected:
//	char *m_name;
//	int m_age;
//
//public:
//	People(char *name, int age);
//	virtual void display();
//};
//
//
//People::People(char *name, int age) :m_name(name), m_age(age){}
//void People::display()
//{
//	cout << m_name << ", age is: " << m_age << endl;
//}
//
//
//class Teacher : public People {
//
//private:
//	int m_salary;
//public:
//	Teacher(char *name, int age, int salary);
//	virtual void display();
//};
//
//Teacher::Teacher(char *name, int age, int salary) : People(name, age), m_salary(salary){}
//void Teacher::display()
//{
//	cout << m_name << ", age is: " << m_age << ", salary is: " << m_salary << endl;
//}
//
//int main()
//{
//	/*People *p = new People("xp.chen", 22);
//	p->display();
//
//	p = new Teacher("hy.gao", 21, 10000);
//	p->display();*/
//
//	People p("xp.chen", 20);
//	Teacher t("hy.gao", 19, 8200);
//
//	People &rp = p;
//	Teacher &rt = t;
//	rp.display();
//	rt.display();
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
//
//
//
