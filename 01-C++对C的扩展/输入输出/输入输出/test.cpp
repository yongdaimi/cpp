#include <iostream>
#include <string>
#include <stdlib.h>

int main1()
{
	using namespace std;
	string str;
	int age;
	cin >> str >> age;
	cout << str << "已经成立了" << age << "年了!" << endl;
	system("pause");
	return 0;
}

using namespace std;


void func()
{
	using namespace std;
	cout << "http://www.taobao.com" << endl;
}


int main2()
{
	// using namespace std;
	cout << "cpp" << endl;
	func();

	return 0;
}


int main()
{
	cout << "java" << endl;
	func();
	system("pause");
	return 0;
}

