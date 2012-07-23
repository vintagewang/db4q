/*
 * $Id: AtomicBoolean.cpp 1 2012-01-17 02:31:57Z  $
 */
#include "AtomicBoolean.h"

namespace LWPR
{
#define BOOL_2_INT(b)		((b) ? 1 : 0)
#define INT_2_BOOL(n)		((n != 0) ? true : false)

	AtomicBoolean::AtomicBoolean(bool initialValue)
		: m_nValue(BOOL_2_INT(initialValue))
	{
	}

	bool AtomicBoolean::CompareAndSet(bool expect, bool update)
	{
		return __sync_bool_compare_and_swap(&m_nValue, BOOL_2_INT(expect), BOOL_2_INT(update));
	}

	bool AtomicBoolean::Get() const
	{
		return INT_2_BOOL(m_nValue);
	}

	bool AtomicBoolean::GetAndSet(bool newValue)
	{
		while(true)
		{
			bool current = Get();
			if(CompareAndSet(current, newValue))
				return current;
		}
	}

	void AtomicBoolean::Set(bool newValue)
	{
		m_nValue = BOOL_2_INT(newValue);
	}

	AtomicBoolean::operator bool () const
	{
		return Get();
	}
};
