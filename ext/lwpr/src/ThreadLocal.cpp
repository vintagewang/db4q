/*
 * $Id: ThreadLocal.cpp 1 2012-01-17 02:31:57Z  $
 */
#include "ThreadLocal.h"

namespace LWPR
{
	ThreadLocal::ThreadLocal()
		: m_Key(0)
	{
		int retcode = 0;

		retcode = pthread_key_create(&m_Key, KeyDestructor);
		if(retcode != 0)
			throw LWPR::LWPR_THREAD_API_ERR(EXCEPTION_THREAD_TRACE, retcode, "pthread_key_create error");
	}

	ThreadLocal::~ThreadLocal()
	{
		pthread_key_delete(m_Key);
	}

	TLSValue* ThreadLocal::GetValue()
	{
		void* p = pthread_getspecific(m_Key);
		return static_cast < TLSValue* > (p);
	}

	void ThreadLocal::SetValue(TLSValue* value)
	{
		int retcode = 0;

		retcode = pthread_setspecific(m_Key, (void *)value);
		if(retcode != 0)
			throw LWPR::LWPR_THREAD_API_ERR(EXCEPTION_THREAD_TRACE, retcode, "pthread_setspecific error");
	}

	void ThreadLocal::KeyDestructor(void* value)
	{
		TLSValue* p = static_cast < TLSValue* > (value);
		delete p;
	}
};
