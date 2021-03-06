/*
 * $Id: test1.cpp 1 2012-01-17 02:31:57Z  $
 */
/*
 * 测试LWPR::TimerThread类是否能正常执行定时
 */
#include "TimerThread.h"
#include <iostream>
#include <stdio.h>

using namespace std;

class MyReq : public LWPR::WorkRequest
{
public:
	void DoWork()
	{
		cout << "Oh I am working" << endl;
	}
};

/**
 * 主程序
 */
int main(int argc, char *argv[])
{
	LWPR::TimerThread t;

	MyReq* p =  new MyReq;

	// 添加请求
	t.AddTimerRequest(p, 3);

	sleep(1);

	t.AddTimerRequest(p, 6);

	sleep(1000000);
	return 0;
}
