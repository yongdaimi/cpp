


int main()
{

	/*int *p = (int *)malloc(sizeof(int) * 10);
	free(p);*/

	// C++������new��delete�ؼ�������̬�����ڴ棬 ��C++�н���ʹ��new��delete�������ڴ棬���ǿ����Զ����ù��캯������������

	/* һ������ */
	int *p = new int; // ����1��int���͵��ڴ�ռ䣬new����������ݺ���������������ƶ�����ռ�Ĵ�С��
	delete p; // �ͷ��ڴ�

	int *q = new int[10]; // ����10��int���Ϳռ�
	delete[] q;

	return 0;
}