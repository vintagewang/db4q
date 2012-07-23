/*
 * $Id: Semph.cpp 1 2012-01-17 02:31:57Z  $
 */
#include "Semph.h"
#include <assert.h>

namespace LWPR
{

	Semph::Semph(int value /*= 1*/)
	{
		assert(value >= 1);

		int retcode = 0;

		retcode = sem_init(&m_sem, 0, value);
		if(retcode != 0)
			throw LWPR::LWPR_THREAD_SEM_INIT_ERR(EXCEPTION_THREAD_TRACE, retcode, "sem_init error");
	}

	Semph::~Semph()
	{
		sem_destroy(&m_sem);
	}

	void Semph::Lock()
	{
		int retcode = 0;

		retcode = sem_wait(&m_sem);
		if(retcode != 0)
			throw LWPR::LWPR_THREAD_SEM_WAIT_ERR(EXCEPTION_THREAD_TRACE, retcode, "sem_wait error");
	}

	void Semph::Unlock()
	{
		int retcode = 0;

		retcode = sem_post(&m_sem);
		if(retcode != 0)
			throw LWPR::LWPR_THREAD_SEM_POST_ERR(EXCEPTION_THREAD_TRACE, retcode, "sem_post error");
	}
};
