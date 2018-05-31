#include <iostream>
#include <stdlib.h>

using namespace std;

void Swap(int *a, int *b)
{
	int tmp = *a;
	*a = *b;
	*b = tmp;
}

void Swap(float *a, float *b)
{
	float tmp = *a;
	*a = *b;
	*b = tmp;
}

void Swap(char *a, char *b)
{
	char tmp = *a;
	*a = *b;
	*b = tmp;
}

void Swap(bool *a, bool *b){
	bool temp = *a;
	*a = *b;
	*b = temp;
}


int main1()
{
	//���� int ������ֵ
	int n1 = 100, n2 = 200;
	Swap(&n1, &n2);
	cout << n1 << ", " << n2 << endl;

	//���� float ������ֵ
	float f1 = 12.5, f2 = 56.93;
	Swap(&f1, &f2);
	cout << f1 << ", " << f2 << endl;

	//���� char ������ֵ
	char c1 = 'A', c2 = 'B';
	Swap(&c1, &c2);
	cout << c1 << ", " << c2 << endl;

	//���� bool ������ֵ
	bool b1 = false, b2 = true;
	Swap(&b1, &b2);
	cout << b1 << ", " << b2 << endl;

	system("pause");

	return 0;
}
