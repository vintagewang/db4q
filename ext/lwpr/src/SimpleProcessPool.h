/*
 * $Id: SimpleProcessPool.h 1 2012-01-17 02:31:57Z  $
 */
#ifndef LWPR_SIMPLE_PROCESS_POOL_H__
#define LWPR_SIMPLE_PROCESS_POOL_H__
#include "LWPRType.h"
#include <list>

namespace LWPR
{
	/**
	 * 简单进程池功能，能创建固定数目的进程，并维护进程数，当有进程以外退出，则重启
	 */
	class SimpleProcessPool
	{
	public:
		/**
		 * 构造函数
		 */
		SimpleProcessPool();

		/**
		 * 析构函数
		 */
		virtual ~SimpleProcessPool();

		/**
		 * 设置启动进程数量
		 */
		void SetProcessNum(int process_num);

		/**
		 * 开始运行
		 */
		void Run(int argc, char** argv);

		/**
		 * 子进程执行体
		 */
		virtual int DoChildWork(int argc, char** argv) = 0;

	private:
		int m_nProcessNum;
		std::list < int > m_ProcessList;
	};
};

#endif // end of LWPR_SIMPLE_PROCESS_POOL_H__
