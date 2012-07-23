/*
 * $Id: AtomicBoolean.h 1 2012-01-17 02:31:57Z  $
 */
#ifndef LWPR_ATOMIC_BOOLEAN_H__
#define LWPR_ATOMIC_BOOLEAN_H__
#include "LWPRType.h"

namespace LWPR
{
	class AtomicBoolean
	{
	public:
		/**
		 * 构造函数
		 */
		AtomicBoolean(bool initialValue = false);

		/**
		 * 如果当前值 == 预期值，则以原子方式将该值设置为给定的更新值
		 * @return 如果成功，则返回 true。返回 False 指示实际值与预期值不相等
		 */
		bool CompareAndSet(bool expect, bool update);

		/**
		 * 返回当前值
		 */
		bool Get() const;

		/**
		 * 设置为给定值，并返回以前的值
		 * @return 以前的值
		 */
		bool GetAndSet(bool newValue);

		/**
		 * 无条件地设置为给定值。
		 */
		void Set(bool newValue);

		/**
		 * 类型转化操作符
		 */
		operator bool () const;

	private:
		volatile int m_nValue;
	};
};

#endif // end of LWPR_ATOMIC_BOOLEAN_H__
