#include <iostream>
#include <stdlib.h>
using namespace std;

template<class T1, class T2>
class Point {
	
private:
	T1 m_x;
	T2 m_y;

public:
	Point(T1 x, T2 y) :m_x(x), m_y(y){}
	T1 getX() const;
	void setX(T1 x);
	T2 getY() const;
	void setY(T2 y);
	void display() const;
};


//template<class T1, class T2>
//void T1 Point<T1, T2>::getX() const
//{
//	return m_x;
//}
//
//template<class T1, class T2>
//void Point<T1, T2>::setX(T1 x)
//{
//	m_x = x;
//}
//
//template<class T1, class T2>
//void T2 Point<T1, T2>::getY() const
//{
//	return m_y;
//}
//
//template<class T1, class T2>
//void Point<T1, T2>::setY(T2 y)
//{
//	m_y = y;
//}

template<class T1, class T2>
void Point<T1, T2>::display() const
{
	cout << "x = " << m_x << ", y = " << m_y << endl;
}

template<>
class Point<char *, char *> {

private:
	char *m_x;
	char *m_y;

public:
	Point(char *x, char *y) :m_x(x), m_y(y){}
	char *getX() const;
	void setX(char *x);
	char *getY() const;
	void setY(char *y);
	void display() const;
};

// 这里不带模板头
void Point<char*, char*>::display() const
{
	cout << "x = " << m_x << ", y = " << m_y << endl;
}


int main()
{
	(new Point<int, int>(10, 20))->display();
	(new Point<int, char *>(10, "东经120度"))->display();
	(new Point<char*, char*>("东经180度","北纬210度"))->display();

	system("pause");
	return 0;
}























