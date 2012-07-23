/*
 * $Id: ThreadLocal.h 1 2012-01-17 02:31:57Z  $
 */
#ifndef LWPR_THREAD_LOCAL_H__
#define LWPR_THREAD_LOCAL_H__
#include "LWPRType.h"
#include "Thread.h"

namespace LWPR
{
	class TLSValue
	{
	public:
		virtual ~TLSValue() {};
	};

	class ThreadLocal
	{
	public:
		ThreadLocal();
		virtual ~ThreadLocal();

		TLSValue* GetValue();
		void SetValue(TLSValue* value);

	private:
		static void KeyDestructor(void* value);
		THREAD_KEY_T	m_Key;
	};
};

#endif // end of LWPR_THREAD_LOCAL_H__
