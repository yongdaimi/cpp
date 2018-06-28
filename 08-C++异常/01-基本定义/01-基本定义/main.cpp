#include <iostream>
#include <stdlib.h>
using namespace std;


class Base{};
class Derived : public Base{};


int main()
{

	int nums[] = { 1, 2, 3 };
	try{
		throw nums;
		cout << "This statement will not be executed." << endl;
	}
	catch (const int *){
		cout << "Exception type: const int *" << endl;
	}

	system("pause");
	return 0;
}



