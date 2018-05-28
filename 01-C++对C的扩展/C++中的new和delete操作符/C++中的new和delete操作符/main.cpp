


int main()
{

	/*int *p = (int *)malloc(sizeof(int) * 10);
	free(p);*/

	// C++新增了new和delete关键字来动态分配内存， 在C++中建议使用new和delete来管理内存，它们可以自动调用构造函数和析构函数

	/* 一个数据 */
	int *p = new int; // 分配1个int类型的内存空间，new操作符会根据后面的数据类型在推断所需空间的大小。
	delete p; // 释放内存

	int *q = new int[10]; // 分配10个int类型空间
	delete[] q;

	return 0;
}