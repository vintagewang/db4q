/*
 * $Id: Semph.h 1 2012-01-17 02:31:57Z  $
 */
#ifndef LWPR_SEMPH_H__
#define LWPR_SEMPH_H__
#include "LWPRType.h"
#include "Exception.h"
#include <semaphore.h>

namespace LWPR
{
	typedef sem_t SEM_T;

	class Semph
	{
	public:

		/**
		 * 构造函数
		 */
		Semph(int value = 1);

		/**
		 * 析构函数
		 */
		~Semph();

		/**
		 * 加锁
		 */
		void Lock();

		/**
		 * 释放锁
		 */
		void Unlock();

	private:
		SEM_T m_sem;
	};

	DEFINE_EXCEPTION(LWPR_THREAD_SEM_INIT_ERR);
	DEFINE_EXCEPTION(LWPR_THREAD_SEM_POST_ERR);
	DEFINE_EXCEPTION(LWPR_THREAD_SEM_WAIT_ERR);
};

#endif // end of LWPR_SEMPH_H__
