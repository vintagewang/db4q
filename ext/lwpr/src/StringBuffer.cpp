/*
 * $Id: StringBuffer.cpp 1 2012-01-17 02:31:57Z  $
 */
#include "StringBuffer.h"
#include "StringUtil.h"
#include <cstring>

namespace LWPR
{
	StringBuffer::StringBuffer()
		: Buffer()
	{
	}

	StringBuffer::StringBuffer(int capacity /*= BUFFER_DEFAULT_CAPACITY*/)
		: Buffer(capacity)
	{

	}

	StringBuffer::StringBuffer(char* str)
	{
		*this = str;
	}

	StringBuffer::StringBuffer(const char* str)
	{
		*this = str;
	}

	StringBuffer::StringBuffer(const Buffer& buf)
	{
		*this = buf;
	}

	StringBuffer::StringBuffer(const StringBuffer& strbuf)
	{
		*this = strbuf;
	}


	void StringBuffer::operator=(char* str)
	{
		if(str == NULL) return;

		this->Capacity(strlen(str) + 1);
		strcpy(this->Inout(), str);
	}

	void StringBuffer::operator=(const char* str)
	{
		if(str == NULL) return;

		this->Capacity(strlen(str) + 1);
		strcpy(this->Inout(), str);
	}

	void StringBuffer::operator=(const Buffer& buf)
	{
		this->Capacity(buf.Capacity() + 1);
		char *src = buf.Inout();
		char *dst = this->Inout();
		for(int i = 0; i < buf.Capacity() && (*dst++ = *src++); i++);
		*dst = 0;
	}

	void StringBuffer::operator=(const StringBuffer& strbuf)
	{
		this->Capacity(strbuf.Capacity());
		strcpy(this->Inout(), strbuf.Inout());
	}

	char* StringBuffer::str() const
	{
		return this->Inout();
	}

	int StringBuffer::Length() const
	{
		return strlen(this->Inout());
	}

	int StringBuffer::FindFirstOf(char ch, int index /*= 0*/) const
	{
		int nLen = Length();

		for(int i = index; i < nLen; i++)
		{
			if(*(this->Inout() + i) == ch)
			{
				return i;
			}
		}

		return -1;
	}

	char* StringBuffer::Trim()
	{
		return LWPR::StringUtil::TrimAll(this->Inout());
	}

	char* StringBuffer::Append(const char* str)
	{
		if(str != NULL)
		{
			this->Capacity(Length() + strlen(str) + 1);
			strcat(this->Inout(), str);
		}

		return this->Inout();
	}
};
