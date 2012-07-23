/*
 * $Id: StringBuffer.h 1 2012-01-17 02:31:57Z  $
 */
#ifndef LWPR_STRING_BUFFER_H__
#define LWPR_STRING_BUFFER_H__
#include "LWPRType.h"
#include "Buffer.h"

namespace LWPR
{
	class StringBuffer : public Buffer
	{
	public:
		/**
		 * 构造函数
		 */
		StringBuffer();
		StringBuffer(int capacity = BUFFER_DEFAULT_CAPACITY);
		StringBuffer(char* str);
		StringBuffer(const char* str);
		StringBuffer(const Buffer& buf);
		StringBuffer(const StringBuffer& strbuf);

		/**
		 * 赋值操作符重载
		 */
		void operator= (char* str);
		void operator= (const char* str);
		void operator= (const Buffer& buf);
		void operator= (const StringBuffer& strbuf);

		/**
		 * 向字符串末端追加
		 */
		char* Append(const char* str);

		/**
		 * 返回字符串指针
		 */
		char* str() const;

		/**
		 * 返回字符串的长度
		 */
		int Length() const;

		/**
		 * 去除前后空格、制表符、换行符
		 */
		char* Trim();

		/**
		 * 查找ch在字符串中第一次出现的位置
		 */
		int FindFirstOf(char ch, int index = 0) const;

	private:
	};

	DECLAREVAR(StringBuffer);
};

#endif // end of LWPR_STRING_BUFFER_H__
