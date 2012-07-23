/*
 * $Id: Object.cpp 1 2012-01-17 02:31:57Z  $
 */
#include "Object.h"

namespace LWPR
{
	Object* Object::Duplicate(Object *ref)
	{
		if(ref)
		{
			ref->IncRef();
		}
		return ref;
	}

	void Object::Release(Object *ref)
	{
		if(ref)
		{
			ref->DecRef();
		}
	}

	Object::~Object()
	{
		// 如果没有如下判断，则对象可以在栈上定义
		//assert(m_nRefCount.Get() == 0);
	}

	void Object::IncRef()
	{
		assert(m_nRefCount >= 1);
		m_nRefCount++;
	}

	void Object::DecRef()
	{
		assert(m_nRefCount > 0);
		if(--m_nRefCount == 0)
		{
			delete this;
		}
	}
};
