#include <stdio.h>
#include <stdlib.h>


namespace Diy {
	class Student {
	public:
		char *name;
		int age;
		float score;

	public:
		void say() {
			printf("%s�������ǣ�%d, �ɼ��ǣ�%f\n",name, age, score);
		}
	};

}


int main()
{
	Diy::Student stu;
	stu.name = "xp";
	stu.age = 15;
	stu.score = 92.5f;
	stu.say();

	system("pause");
	return 0;
}





