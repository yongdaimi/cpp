#include <stdio.h>
#include <stdlib.h>

class Student {
public:
	char *name;
	int age;
	float score;

	void say() {
		printf("%s��������%d, �ɼ���%f\n", name, age, score);
	}
};

int main()
{
	class Student stu1;
	stu1.name = "xp";
	stu1.age = 16;
	stu1.score = 92.5f;
	stu1.say();
	system("pause");
	return 0;
}