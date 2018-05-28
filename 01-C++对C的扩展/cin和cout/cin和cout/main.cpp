#include <iostream>

using namespace std;

int main1()
{
	int x;
	float y;
	cout << "Please input an int number:" << endl;
	cin >> x;
	cout << "The int number is x=" << x << endl;
	cout << "Please input a float number:" << endl;
	cin >> y;
	cout << "The float number is y=" << y << endl;

	system("pause");

	return 0;
}


int main()
{
	int x;
	float y;
	cout << "Please input an int number and a float number:" << endl;
	cin >> x >> y;
	cout << "The int number is x= " << x << endl;
	cout << "The float number is y= " << y << endl;

	system("pause");

	return 0;
}