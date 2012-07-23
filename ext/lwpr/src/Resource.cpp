/*
 * $Id: Resource.cpp 1 2012-01-17 02:31:57Z  $
 */
#include "Resource.h"
#include "Thread.h"
#include "Synchronized.h"
#include <sys/time.h>
#include <time.h>

namespace LWPR
{
	Resource::Resource()
		: m_nWaitThreads(0), m_nResource(0)
	{
		int retcode = pthread_cond_init(&m_cond, NULL);
		if(retcode != 0)
			throw LWPR::LWPR_THREAD_API_ERR(EXCEPTION_THREAD_TRACE, retcode, "pthread_cond_init error");
	}

	Resource::~Resource()
	{
		pthread_cond_destroy(&m_cond);
	}

	void Resource::Wait()
	{
		int retcode = 0;

		Synchronized syn(m_mutex);

		m_nWaitThreads++;

		while(m_nResource == 0)
		{
			retcode = pthread_cond_wait(&m_cond, m_mutex.GetMutexRef());
			if(retcode != 0)
			{
				m_nWaitThreads--;
				throw LWPR::LWPR_THREAD_API_ERR(EXCEPTION_THREAD_TRACE, retcode, "pthread_cond_wait error");
			}
		}

		if(retcode == 0 && m_nResource > 0)
		{
			if(m_nWaitThreads == 1)
			{
				m_nResource = 0;
			}
			else if(m_nWaitThreads > 1)
			{
				m_nResource--;
			}
		}

		m_nWaitThreads--;
	}

	void Resource::Wait(int seconds)
	{
		assert(seconds >= 0);

		int retcode = 0;

		Synchronized syn(m_mutex);

		m_nWaitThreads++;

		while(m_nResource == 0)
		{
			struct timespec tv = {0};
			struct timeval tv2 = {0};

			gettimeofday(&tv2, NULL);
			tv.tv_sec = tv2.tv_sec + (time_t) seconds;

			retcode = pthread_cond_timedwait(&m_cond, m_mutex.GetMutexRef(), &tv);
			if(retcode == ETIMEDOUT)
			{
				break;
			}
			else if(retcode != 0)
			{
				m_nWaitThreads--;
				throw LWPR::LWPR_THREAD_API_ERR(EXCEPTION_THREAD_TRACE, retcode, "pthread_cond_timedwait error");
			}
		}

		if(retcode == 0 && m_nResource > 0)
		{
			if(m_nWaitThreads == 1)
			{
				m_nResource = 0;
			}
			else if(m_nWaitThreads > 1)
			{
				m_nResource--;
			}
		}

		m_nWaitThreads--;
	}

	void Resource::Notify()
	{
		int retcode = 0;

		Synchronized syn(m_mutex);

		retcode = pthread_cond_signal(&m_cond);
		if(retcode != 0) throw LWPR::LWPR_THREAD_API_ERR(EXCEPTION_THREAD_TRACE, retcode, "pthread_cond_signal error");

		m_nResource += 1;
	}

	void Resource::Notifyall()
	{
		int retcode = 0;

		Synchronized syn(m_mutex);

		if(m_nWaitThreads > 0)
		{
			retcode = pthread_cond_broadcast(&m_cond);
			if(retcode != 0) throw LWPR::LWPR_THREAD_API_ERR(EXCEPTION_THREAD_TRACE, retcode, "pthread_cond_broadcast error");
			m_nResource += m_nWaitThreads;
		}
	}
};
