/*
 * $Id: TimerThread.cpp 1 2012-01-17 02:31:57Z  $
 */
#include "TimerThread.h"
#include "Synchronized.h"
#include <sys/time.h>

namespace LWPR
{
	//----------------------------------------------------------------------------------
	// class timer_request
	//----------------------------------------------------------------------------------
	bool TimerThread::timer_request::operator<(timer_request& right)
	{
		return this->nTimestamp < right.nTimestamp;
	}

	//----------------------------------------------------------------------------------
	// class TimerThread
	//----------------------------------------------------------------------------------
	TimerThread::TimerThread()
	{
		Start();
	}

	TimerThread::~TimerThread()
	{
	}

	void TimerThread::AddTimerRequest(WorkRequest* preq, int seconds)
	{
		if(preq != NULL)
		{
			struct timeval tv = {0};
			gettimeofday(&tv, NULL);

			TIMER_REQUEST_T timer = {0};
			timer.pRequest = preq;
			timer.nTimestamp = tv.tv_sec + seconds;

			{
				Synchronized syn(m_Mutex);
				m_TimerRequestList.push_back(timer);
			}

			Notify();
		}
	}

	void TimerThread::DoTimerRequest(WorkRequest* preq)
	{
		if(preq != NULL)
		{
			preq->DoWork();
		}
	}

	void TimerThread::Run()
	{
		while(IsContinue())
		{
			TIMER_REQUEST_T tTimerReq = {0};
			bool bHasReq = false;
			bool bReqExpired = false;
			int nDiffTime = 0;

			// 取一个Timer请求
			{
				Synchronized syn(m_Mutex);
				m_TimerRequestList.sort();

				TimerRequestList::iterator it = m_TimerRequestList.begin();
				if(it != m_TimerRequestList.end())
				{
					tTimerReq = *it;
					bHasReq = true;

					struct timeval tv = {0};
					gettimeofday(&tv, NULL);

					if(tv.tv_sec >= tTimerReq.nTimestamp)
					{
						m_TimerRequestList.pop_front();
						bReqExpired = true;
					}
					else
					{
						nDiffTime = tTimerReq.nTimestamp - tv.tv_sec;
					}
				}
			}

			// 对请求执行定时
			if(bHasReq)
			{
				if(bReqExpired)
				{
					DoTimerRequest(tTimerReq.pRequest);
				}
				else
				{
					Wait(nDiffTime);
				}
			}
			else
			{
				Wait();
			}
		}
	}
};
