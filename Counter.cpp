#include <iostream>
using namespace std;

class Counter
{
	friend ostream& operator<<(ostream& cout, Counter p);
public:
	Counter(int n)
	{
		num = n;
	}
	//��������
	Counter& operator++()//ǰ��//ע��һ��Ҫ�������õķ�ʽ������
		//����ٿصľͲ���ͬһ���ڴ�ռ��ˣ����ݾͻ����
	{
		num++;//ǰ�õ��������Ӧ���Ƚ���++�Ĳ������ٽ�������з���
		return *this;
	}
	//ע��ǰ�úͺ��õ���������
	//Ϊ������ǰ��������ͺ����������++
	//���õ�����������β��б�����һ��int
	Counter operator++(int)//���ã�intռλ����
	{
		//�ȼ�¼��ǰ���
		Counter temp = *this;//����ǰ��ֵ��һ������
		//�Ա�֮����Է��أ��ٽ���һ�������Ĳ���
		num++;
		return temp;
	}

	int num;//��Ŀ�����
};

ostream& operator<<(ostream& cout, Counter p)
{
	cout << p.num << endl;
	return cout;
}



int main()
{
	int n;
	cin >> n;
	Counter c1(n);
	cout << c1++;
	cout << ++c1;


	return 0;
}
