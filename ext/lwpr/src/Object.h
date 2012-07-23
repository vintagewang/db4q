/*
 * $Id: Object.h 1 2012-01-17 02:31:57Z  $
 */
#ifndef LWPR_OBJECT_H__
#define LWPR_OBJECT_H__
#include "LWPRType.h"
#include "AtomicInteger.h"

namespace LWPR
{
	class Object
	{
	public:
		/**
		 * 对象引用个数加1
		 */
		static Object* Duplicate(Object *ref);
		/**
		 * 对象引用个数减1，当减为0时，释放对象
		 */
		static void Release(Object *ref);

		void IncRef();
		void DecRef();

	protected:
		Object() : m_nRefCount(1) {}
		virtual ~Object();

	private:
		AtomicInteger m_nRefCount;
	};
};

#endif // end of LWPR_OBJECT_H__
