#include <iostream>
using namespace std;

int sum(int n)
{
	int total = 0;
	for (int i = 0; i <= n; i++) {
		total += i;
	}
	return total;
}


int main()
{
	cout << "Input a number:" << endl;
	int n;
	cin >> n; 
	cout << "Total: " << sum(n) << endl;
	system("pause");
	return 0;
}

