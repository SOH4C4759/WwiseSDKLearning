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
	//返回引用
	Counter& operator++()//前置//注意一定要按照引用的方式来返回
		//否则操控的就不是同一块内存空间了，数据就会出错
	{
		num++;//前置递增运算符应该先进行++的操作，再将自身进行返回
		return *this;
	}
	//注意前置和后置递增的区分
	//为了区分前置运算符和后置运算符的++
	//后置递增运算符的形参列表中有一个int
	Counter operator++(int)//后置，int占位参数
	{
		//先记录当前结果
		Counter temp = *this;//将当前的值做一个保存
		//以便之后可以返回，再进行一个递增的操作
		num++;
		return temp;
	}

	int num;//单目运算符
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
