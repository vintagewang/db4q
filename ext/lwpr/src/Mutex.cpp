/*
 * $Id: Mutex.cpp 1 2012-01-17 02:31:57Z  $
 */
#include "Mutex.h"
#include "Thread.h"

namespace LWPR
{

	Mutex::Mutex()
	{
		int retcode = 0;

		retcode = pthread_mutex_init(&m_mutex, NULL);
		if(retcode != 0)
			throw LWPR::LWPR_THREAD_API_ERR(EXCEPTION_THREAD_TRACE, retcode, "pthread_mutex_init error");
	}

	Mutex::~Mutex()
	{
		pthread_mutex_destroy(&m_mutex);
	}

	void Mutex::Lock()
	{
		int retcode = 0;

		retcode = pthread_mutex_lock(&m_mutex);
		if(retcode != 0)
			throw LWPR::LWPR_THREAD_API_ERR(EXCEPTION_THREAD_TRACE, retcode, "pthread_mutex_lock error");
	}

	bool Mutex::TryLock()
	{
		int retcode = 0;

		retcode = pthread_mutex_trylock(&m_mutex);

		return (retcode == 0);
	}

	void Mutex::Unlock()
	{
		int retcode = 0;

		retcode = pthread_mutex_unlock(&m_mutex);
		//if(retcode != 0)
		//	throw LWPR::LWPR_THREAD_API_ERR(EXCEPTION_THREAD_TRACE, retcode, "pthread_mutex_unlock error");
	}

	PTHREAD_MUTEX_T* Mutex::GetMutexRef()
	{
		return &m_mutex;
	}
};
