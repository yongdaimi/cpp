#include <iostream>
#include <stdlib.h>
using namespace std;
//����
class Troops{
public:
	virtual void fight(){ cout << "Strike back!" << endl; }
};

//½��
class Army : public Troops{
public:
	void fight(){ cout << "--Army is fighting!" << endl; }
};

//99A��ս̹��
class _99A : public Army{
public:
	void fight(){ cout << "----99A(Tank) is fighting!" << endl; }
};

//��ֱ10��װֱ����
class WZ_10 : public Army{
public:
	void fight(){ cout << "----WZ-10(Helicopter) is fighting!" << endl; }
};

//����10Ѳ������
class CJ_10 : public Army{
public:
	void fight(){ cout << "----CJ-10(Missile) is fighting!" << endl; }
};

//�վ�
class AirForce : public Troops{
public:
	void fight(){ cout << "--AirForce is fighting!" << endl; }
};

//J-20���μ߻���
class J_20 : public AirForce{
public:
	void fight(){ cout << "----J-20(Fighter Plane) is fighting!" << endl; }
};

//CH5���˻�
class CH_5 : public AirForce{
public:
	void fight(){ cout << "----CH-5(UAV) is fighting!" << endl; }
};

//��6K��ը��
class H_6K : public AirForce{
public:
	void fight(){ cout << "----H-6K(Bomber) is fighting!" << endl; }
};


int main01(){
	Troops *p = new Troops;
	p->fight();
	//½��
	p = new Army;
	p->fight();
	p = new _99A;
	p->fight();
	p = new WZ_10;
	p->fight();
	p = new CJ_10;
	p->fight();
	//�վ�
	p = new AirForce;
	p->fight();
	p = new J_20;
	p->fight();
	p = new CH_5;
	p->fight();
	p = new H_6K;
	p->fight();

	system("pause");
	return 0;
}
