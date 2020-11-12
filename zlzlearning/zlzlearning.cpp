// zlzlearning.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
using namespace std;

//子线程循环 10 次，接着主线程循环 100 次，接着又回到子线程循环 10 次，
//接着再回到主线程又循环 100 次，如此循环50次，试写出代码

int flag1 = 10;
mutex mu;
condition_variable cv;
void fun1(int x, char c)
{
	for (int i = 0; i < 50; ++i)
	{
		unique_lock<mutex> lck(mu);
		while (flag1 != x)
		{
			cv.wait(lck);
		}
		for (int j = 0; j < x; ++j)
		{
			cout << c << ":" << j << endl;
		}
		flag1 = (x == 10) ? 100 : 10;
		cv.notify_one();
	}
}
void main_fun1()
{
	thread t1(fun1, 10, 'A');
	fun1(100, 'B');

	t1.join();
}

//编写一个程序，开启3个线程，
//这3个线程的ID分别为A、B、C，每个线程将自己的ID在屏幕上打印10遍，
//要求输出结果必须按ABC的顺序显示；如：ABCABC….依次递推。
int flag2 = 0;
void fun2(int x)
{
	for (int i = 0; i < 10; ++i)
	{
		unique_lock<mutex> lck(mu);
		while (x != flag2)
		{
			cv.wait(lck);
		}
		cout << char('A' + x) << endl;
		flag2 = (flag2 + 1) % 3;
		cv.notify_all();
	}
}
void main_fun2()
{
	thread t1(fun2, 1);
	thread t2(fun2, 2);
	fun2(0);
	t1.join();
	t2.join();
}

//单例模式
mutex mtx;

//懒汉模式
class CSingleton
{
private:
	CSingleton() //构造函数是私有的
	{
	}
	static CSingleton *Instance;

public:
	static CSingleton *GetInstance()
	{
		if (Instance == NULL)
		{
			mtx.lock();
			if (Instance == NULL)
				Instance = new CSingleton();
			cout << "This is singleton" << endl;
			mtx.unlock();
		}

		return Instance;
	}
	void print()
	{
		cout << "aaa" << endl;
	}
};
CSingleton *CSingleton::Instance = 0; //类静态变量需要在类外初始化

//饿汉模式
class Singleton
{
public:
	static Singleton *GetInstance()
	{
		return single;
	}

private:
	Singleton(Singleton const &) = delete;
	Singleton &operator=(Singleton const &) = delete;
	Singleton(){};
	static Singleton *single;
};
Singleton *Singleton::single = new Singleton; // 在程序入口之前就完成单例对象的初始化(这里的new 可以调用类内私有的构造函数)

int main()
{

	cout << "hello!" << endl;
}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门提示:
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
