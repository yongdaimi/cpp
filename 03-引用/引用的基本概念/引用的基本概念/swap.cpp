#include <iostream>
#include <stdlib.h>

using namespace std;


void swap1(int a, int b);
void swap2(int *p1, int *p2);
void swap3(int &a, int &b); 

int main2()
{

	int num1, num2;
	cout << "Input two integers: ";
	cin >> num1 >> num2;
	swap1(num1, num2);
	cout << num1 << " " << num2 << endl;

	cout << "Input two integers: ";
	cin >> num1 >> num2;
	swap2(&num1,&num2);
	cout << num1 << " " << num2 << endl;

	cout << "Input two integers: ";
	cin >> num1 >> num2;
	swap3(num1, num2);
	cout << num1 << " " << num2 << endl;


	system("pause");
	return 0;
}


void swap1(int a, int b)
{
	int tmp = a;
	a = b;
	b = tmp;
}


void swap2(int *p1, int *p2)
{
	int tmp = *p1;
	*p1 = *p2;
	*p2 = tmp;
}

void swap3(int &a, int &b)
{
	int tmp = a;
	a = b;
	b = tmp;
}


