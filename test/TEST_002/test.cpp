/*
 * $Id: test.cpp 35 2012-01-20 02:22:55Z  $
 */
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <list>
#include <string.h>
#include <sys/time.h>
#include <time.h>
#include "db4q.h"


using std::cout;
using std::cerr;
using std::endl;

#define MSG_SIZE (1024 * 4)

#define MSG_CNT (150000)

char g_bufMsg[MSG_SIZE];


static int run(int argc, char** argv)
{
	srandom(time(NULL));
	memset(g_bufMsg, 'K', MSG_SIZE);


	DB4Q::MessageStore* ms = new DB4Q::MessageStore();

	struct timeval tvBegin = {0};

	DB4Q::INT32 msgSent = 0;
	DB4Q::INT32 msgSentLast = 0;
	gettimeofday(&tvBegin, NULL);


	for(int i = 1; msgSent < MSG_CNT;)
	{
		// 构造消息
		DB4Q::INT32 num = rand() % 30;
		DB4Q::MsgDataVector msgs;
		while(num--)
		{
			DB4Q::MsgData msg = {g_bufMsg, MSG_SIZE};
			msgs.push_back(msg);
		}

		// 发送消息
		for(DB4Q::INT32 beginIndex = 0; beginIndex < msgs.size();)
		{
			DB4Q::UINT32 wroteOffset = 0;
			DB4Q::MapedFile mapedFile;

			DB4Q::INT32 ret = ms->put(msgs, beginIndex, mapedFile, wroteOffset);
			if(ret > 0)
			{
				beginIndex += ret;
				msgSent += ret;
				ms->commitPutAction(mapedFile, wroteOffset, true);
			}
			else
			{
				exit(-1);
			}
		}

		if(msgSent > (i * 30000))
		{
			// 计时
			struct timeval tvEnd = {0};
			gettimeofday(&tvEnd, NULL);

			// 精确到毫秒
			DB4Q::INT64 msBegin = (DB4Q::INT64)tvBegin.tv_sec * 1000;
			msBegin += (tvBegin.tv_usec / 1000);

			DB4Q::INT64 msEnd = (DB4Q::INT64)tvEnd.tv_sec * 1000;
			msEnd += (tvEnd.tv_usec / 1000);

			double tps = (msgSent - msgSentLast) / ((msEnd - msBegin) / 1000.0);

			printf("%3d  TPS: %.2f\n", i, tps);

			//printf("msBegin %ld\n", msBegin);
			//printf("msEnd %ld\n", msEnd);

			i++;
			msgSentLast = msgSent;
			gettimeofday(&tvBegin, NULL);
		}
	}

	//gettimeofday(&tvEnd, NULL);

	//// 统计
	//DB4Q::INT32 sec = (DB4Q::INT32)(tvEnd.tv_sec - tvBegin.tv_sec);
	//DB4Q::INT32 usec = (DB4Q::INT32)(tvEnd.tv_usec - tvBegin.tv_usec);

	//printf("	msgsent %d time: %d:%d\n", msgSent, sec, usec);


	delete ms;
	ms = NULL;

	return 0;
}

int main(int argc, char** argv)
{
	try
	{
		return run(argc, argv);
	}
	catch(...)
	{
		cerr << "Unknow exception" << endl;
	}

	return -1;
}
