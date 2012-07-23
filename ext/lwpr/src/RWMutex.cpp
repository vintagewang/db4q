/*
 * $Id: RWMutex.cpp 1 2012-01-17 02:31:57Z  $
 */
#include "RWMutex.h"
#include "Thread.h"

namespace LWPR
{
	RWMutex::RWMutex()
	{
		int retcode = 0;

		retcode = pthread_rwlock_init(&m_rwlock, NULL);
		if(retcode != 0)
			throw LWPR::LWPR_THREAD_API_ERR(EXCEPTION_THREAD_TRACE, retcode, "pthread_rwlock_init error");
	}

	RWMutex::~RWMutex()
	{
		pthread_rwlock_destroy(&m_rwlock);
	}

	void RWMutex::RDLock()
	{
		int retcode = 0;

		retcode = pthread_rwlock_rdlock(&m_rwlock);
		if(retcode != 0)
			throw LWPR::LWPR_THREAD_API_ERR(EXCEPTION_THREAD_TRACE, retcode, "pthread_rwlock_rdlock error");
	}

	void RWMutex::WRLock()
	{
		int retcode = 0;

		retcode = pthread_rwlock_wrlock(&m_rwlock);
		if(retcode != 0)
			throw LWPR::LWPR_THREAD_API_ERR(EXCEPTION_THREAD_TRACE, retcode, "pthread_rwlock_wrlock error");

	}

	void RWMutex::Unlock()
	{
		int retcode = 0;

		retcode = pthread_rwlock_unlock(&m_rwlock);
		if(retcode != 0)
			throw LWPR::LWPR_THREAD_API_ERR(EXCEPTION_THREAD_TRACE, retcode, "pthread_rwlock_unlock error");
	}
};
