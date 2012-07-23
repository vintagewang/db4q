/*
 * $Id: TimerThread.h 1 2012-01-17 02:31:57Z  $
 */
#ifndef LWPR_TIMER_THREAD_H__
#define LWPR_TIMER_THREAD_H__
#include "LWPRType.h"
#include "Thread.h"
#include "Resource.h"
#include "ThreadPool.h"
#include "Mutex.h"
#include <list>
#include <time.h>

namespace LWPR
{
	class TimerThread :  public Thread, public Resource
	{
		typedef struct timer_request
		{
			WorkRequest* pRequest;
			time_t nTimestamp;

			bool operator < (timer_request& right);
		} TIMER_REQUEST_T;

		typedef std::list< TIMER_REQUEST_T > TimerRequestList;

	public:
		TimerThread();
		virtual ~TimerThread();

		void AddTimerRequest(WorkRequest* preq, int seconds);

	protected:

		virtual void DoTimerRequest(WorkRequest* preq);

	private:

		void Run();

	private:
		Mutex				m_Mutex;
		TimerRequestList	m_TimerRequestList;
	};
};

#endif // end of LWPR_TIMER_THREAD_H__
