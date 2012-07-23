/*
 * $Id: Buffer.h 1 2012-01-17 02:31:57Z  $
 */
#ifndef LWPR_BUFFER_H__
#define LWPR_BUFFER_H__

#include "LWPRType.h"
#include "Object.h"
#include "Var.h"
#include "Exception.h"

namespace LWPR
{
	class StringBuffer;

	const int BUFFER_DEFAULT_CAPACITY = 128;

	class Buffer : public LWPR::Object
	{
	public:

		/**
		 * 构造函数
		 */
		Buffer(int capacity = BUFFER_DEFAULT_CAPACITY);

		/**
		 * 拷贝构造函数
		 */
		Buffer(const Buffer& rhs);

		/**
		 * 赋值操作符重载
		 */
		Buffer& operator= (const Buffer& rhs);

		/**
		 * 析构函数
		 */
		virtual ~Buffer();

		/**
		 * 将所有内容初始化为0
		 */
		void Reset();

		/**
		 * 返回内存块首地址，供调用者读写
		 */
		virtual char *Inout() const;

		/**
		 * 返回分配的内存大小
		 */
		virtual int Capacity() const;

		/**
		 * 重新设置分配的内存大小
		 */
		virtual int Capacity(int capacity);

		/**
		 * 设置内存的有效大小
		 */
		int Size(int sz);

		/**
		 * 返回内存的有效大小
		 */
		int Size() const;

		/**
		 * 复制一份缓冲区
		 */
		Buffer* CopyValueOf(int offset, int count);

		/**
		 * 转化出一份字符串缓冲区
		 */
		StringBuffer* ToString(int offset, int count);

	protected:
		void Realloc(int size);

	protected:
		int     m_capacity;	///< 内存块的大小
		int     m_size;		///< 内存块的有效大小，默认为0
		char    *m_pbuf;	///< 内存块的首地址
	};

	DECLAREVAR(Buffer);

	DEFINE_EXCEPTION(LWPR_MEM_NOT_ENOUGH);
};

#endif // end of LWPR_BUFFER_H__
