#include <iostream>
#include <stdlib.h>
using namespace std;

class Line {

protected:
	float m_len;
public:
	Line(float len);
	virtual float area() = 0;
	virtual float volume() = 0;

};

Line::Line(float len) :m_len(len){}

class Rect : public Line {
protected:
	float m_width;
public:
	Rect(float len, float width);
	float area();
};

Rect::Rect(float len, float width) :Line(len), m_width(width){}

float Rect::area()
{
	return m_len*m_width;
}

class Cuboid : public Rect {
protected:
	float m_height;
public:
	Cuboid(float len, float width, float height);
	float area();
	float volume();
};

Cuboid::Cuboid(float len, float width, float height) : Rect(len, width), m_height(height){}
float Cuboid::area()
{
	return 2 * (m_len*m_width + m_len*m_height+m_width*m_height);
}

float Cuboid::volume()
{
	return m_len* m_width *m_height;
}

class Cube : public Cuboid {
public:
	Cube(float len);
	float area();
	float volume();
};

Cube::Cube(float len) : Cuboid(len, len, len){}
float Cube::area()
{
	return 6 * m_len *m_len;
}

float Cube::volume()
{
	return m_len*m_len*m_len;
}

int main()
{
	Line *p = new Cuboid(10, 20, 30);
	cout << "The area of Cuboid is: " << p->area() << endl;
	cout << "The volume of Cuboid is: " << p->volume() << endl;

	p = new Cube(15);
	cout << "The area of Cube is: " << p->area() << endl;
	cout << "The Volume of Cube is: " << p->volume() << endl;

	system("pause");
	return 0;
}






























