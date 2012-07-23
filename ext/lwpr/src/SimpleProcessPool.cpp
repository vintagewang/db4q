/*
 * $Id: SimpleProcessPool.cpp 1 2012-01-17 02:31:57Z  $
 */
#include "SimpleProcessPool.h"
#include <sys/types.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>

namespace LWPR
{
	SimpleProcessPool::SimpleProcessPool()
		: m_nProcessNum(0)
	{
	}

	SimpleProcessPool::~SimpleProcessPool()
	{
		std::list <int > ::iterator it = m_ProcessList.begin();

		for(; it != m_ProcessList.end(); it++)
		{
			kill(SIGKILL, *it);
		}
	}

	void SimpleProcessPool::SetProcessNum(int process_num)
	{
		m_nProcessNum = process_num;
	}

	void SimpleProcessPool::Run(int argc, char** argv)
	{
		// 创建进程
		for(int i = 0; i < m_nProcessNum; i++)
		{
			pid_t pid = fork();
			// child
			if(pid == 0)
			{
				exit(DoChildWork(argc, argv));
			}
			// father
			else if(pid > 0)
			{
				m_ProcessList.push_back(pid);
			}

		}

		// 如果有进程退出， 则启动
		while(true)
		{
			pid_t pid = wait(NULL);
			if(pid > 0)
			{
				std::list <int > ::iterator it = m_ProcessList.begin();

				for(; it != m_ProcessList.end(); it++)
				{
					if(*it == pid)
					{
						m_ProcessList.erase(it);
						break;
					}
				}

				pid = fork();
				// child
				if(pid == 0)
				{
					exit(DoChildWork(argc, argv));
				}
				// father
				else if(pid > 0)
				{
					m_ProcessList.push_back(pid);
				}
			}
		}
	}
};
