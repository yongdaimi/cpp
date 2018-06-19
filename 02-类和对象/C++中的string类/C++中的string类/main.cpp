#include <iostream>
#include <string>
#include <stdlib.h>

using namespace std;


int main()
{

	//string s1;
	//string s2 = "xp.chen";
	//string s3 = s2;
	//string s4(5, 's');

	//string s = "http://www.baidu.com";
	//int len = s.length();


	//cout << len << endl;

	//const char *c_str = s.c_str();
	//printf("%s\n", c_str);

	string s1 = "first";
	string s2 = "second";
	char *s3 = "third";
	char s4[] = "fourth";
	char ch = '@';

	string s5 = s1 + s2;
	string s6 = s1 + s3;
	string s7 = s1 + s4;
	string s8 = s1 + ch;
	int a = 10;
	// string s9 = s1 + a;

	cout << s5 << endl << s6 << endl << s7 << endl << s8 << endl;

	system("pause");
	return 0;
}


