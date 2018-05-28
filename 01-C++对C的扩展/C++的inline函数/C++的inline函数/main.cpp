#include <iostream>
#include <stdlib.h>

using namespace std;
void inline swap(int *a, int *b)
{
	int tmp;
	tmp = *a;
	*a = *b;
	*b = tmp;
}

int main()
{
	int m, n;
	cin >> m >> n;
	cout << m << ", " << n << endl;
	swap(&m, &n);
	cout << m << ", " << n << endl;

	system("pause");
	return 0;
}
