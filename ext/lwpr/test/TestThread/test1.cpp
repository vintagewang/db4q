/*
 * $Id: test1.cpp 1 2012-01-17 02:31:57Z  $
 */
/*
 * 测试LWPR::Thread类是否能正常创建线程
 * 线程是否能正常工作
 */
#include "Thread.h"
#include <iostream>
#include <stdio.h>

using namespace std;

class ThreadTest : public LWPR::Thread
{
public:
	ThreadTest()
	{
		this->Start();
	}

	~ThreadTest()
	{

	}

	void Run()
	{
		while(true)
		{
			printf("线程[%ld] 正在运行。。。\n", GetId());
			Sleep(2.5);
		}
	}

private:
};

/**
 * 主程序
 */
int main(int argc, char *argv[])
{
	ThreadTest t[100];

	sleep(1000000);
	return 0;
}
