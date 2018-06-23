#include <typeinfo>
#include <iostream>
#include <stdlib.h>

using namespace std;

class Base{};
struct STU{};

class Person {
private:
	char *name;
	int age;
};


int main()
{
	//获取一个普通变量的类型信息
	int n = 100;
	const type_info &nInfo = typeid(n);
	cout << nInfo.name() << " | " << nInfo.raw_name() << " | " << nInfo.hash_code() << endl;

	//获取一个字面量的类型信息
	const type_info &dInfo = typeid(25.65);
	cout << dInfo.name() << " | " << dInfo.raw_name() << " | " << dInfo.hash_code() << endl;

	//获取一个对象的类型信息
	Base obj;
	const type_info &objInfo = typeid(obj);
	cout << objInfo.name() << " | " << objInfo.raw_name() << " | " << objInfo.hash_code() << endl;

	//获取一个类的类型信息
	const type_info &baseInfo = typeid(Base);
	cout << baseInfo.name() << " | " << baseInfo.raw_name() << " | " << baseInfo.hash_code() << endl;

	//获取一个结构体的类型信息
	const type_info &stuInfo = typeid(struct STU);
	cout << stuInfo.name() << " | " << stuInfo.raw_name() << " | " << stuInfo.hash_code() << endl;

	//获取一个普通类型的类型信息
	const type_info &charInfo = typeid(char);
	cout << charInfo.name() << " | " << charInfo.raw_name() << " | " << charInfo.hash_code() << endl;

	//获取一个表达式的类型信息
	const type_info &expInfo = typeid(20 * 45 / 4.5);
	cout << expInfo.name() << " | " << expInfo.raw_name() << " | " << expInfo.hash_code() << endl;


	Person person;
	const type_info &info = typeid(person);
	cout << info.name() << endl;

	system("pause");
	return 0;
}