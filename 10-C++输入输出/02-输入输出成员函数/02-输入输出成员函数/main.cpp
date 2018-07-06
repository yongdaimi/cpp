#include <iostream>
#include <cstdlib>
using namespace std;

int main01()
{

	char *a = "BASIC";
	for (int i = 4; i >= 0; i--)
		putchar(*(a + i));

	putchar('\n');
	system("pause");
	return 0;
}

int main02()
{
	int c;
	cout << "enter a sentence: " << endl;
	while ((c = cin.get()) != EOF)
		cout.put(c);
		
	return 0;
}

int main03()
{
	char c;
	cout << "enter a sentence:" << endl;
	while (cin.get(c))
	{
		cout.put(c);
	}
	cout << "end" << endl;
	return 0;
}


int main04()
{
	char ch[20];
	cout << "enter a sentence:" << endl;
	cin.get(ch, 10, '\\n');
	cout << ch << endl;

	system("pause");
	return 0;
}

int main05()
{

	char ch[20];
	cin.getline(ch, 20, '\n');
	cout << "READ STATEMENT: " <<ch<< endl;

	system("pause");
	return 0;
}













