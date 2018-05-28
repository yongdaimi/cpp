#include <iostream>
#include <stdlib.h>

using namespace std;

int main1()
{
		
	int a, b;
	bool flag;

	cin >> a >> b;

	flag = a > b;
	cout << "flag=" << flag << endl;

	system("pause");

	return 0;
}


int main()
{

	bool flag = true;

	if (flag) {
		cout << "true" << endl;
	}
	else {
		cout << "false" << endl;
	}

	flag = false;

	if (flag) {
		cout << "true" << endl;
	}
	else {
		cout << "false" << endl;
	}
	
	system("pause");

	return 0;
}

