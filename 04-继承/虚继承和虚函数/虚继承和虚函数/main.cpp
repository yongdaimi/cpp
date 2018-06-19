

/*
class A {

protected:
	int m_a;
};


class B : public A{
protected:
	int m_b;
};


class C : public A {
protected:
	int m_c;
};


class D : public B, public C {
private:
	int m_d;
public:
	void setA(int a){ B::m_a = a; } // ÃüÃû³åÍ»
	void setB(int b){ m_b = b; }
	void setC(int c){ m_c = c; }
	void setD(int d){ m_d = d; }
};

int main01()
{
	D d;

	
	return 0;
}
*/