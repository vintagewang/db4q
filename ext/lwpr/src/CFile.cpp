/*
 * $Id: CFile.cpp 1 2012-01-17 02:31:57Z  $
 */
#include "CFile.h"
#include <cassert>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

namespace LWPR
{
	CFile::CFile(const char* fname, const char* mode)
	{
		assert(fname != NULL && mode != NULL);
		m_fp = fopen(fname, mode);
		if(m_fp == NULL)
		{
			throw FileNotFoundException(EXCEPTION_TRACE, "fopen error");
		}
	}

	CFile::~CFile()
	{
		if(m_fp != NULL)
		{
			fclose(m_fp);
		}
	}

	int CFile::FSIZE()
	{
		struct stat bufStat = {0};

		if(fstat(fileno(m_fp), &bufStat) == -1)
		{
			throw IOException(EXCEPTION_TRACE, "fstat error");
		}

		return bufStat.st_size;
	}

	LWPR::Buffer_ptr CFile::LoadFileContent()
	{
		int nSize = this->FSIZE();
		LWPR::Buffer_var vBuf = new Buffer(nSize + 1);
		vBuf->Size(nSize);

		if(fread(vBuf->Inout(), nSize, 1, m_fp) != 1)
		{
			throw IOException(EXCEPTION_TRACE, "fread error");
		}

		return vBuf.retn();
	}

	void CFile::FWRITE(const LWPR::Buffer& buf)
	{
		if(fwrite(buf.Inout(), buf.Size(), 1, m_fp) != 1)
		{
			throw IOException(EXCEPTION_TRACE, "fwrite error");
		}
	}

	void CFile::FWRITE(const LWPR::StringBuffer& strbuf)
	{
		if(fwrite(strbuf.Inout(), strbuf.Length(), 1, m_fp) != 1)
		{
			throw IOException(EXCEPTION_TRACE, "fwrite error");
		}
	}

	AutoCloseFile::AutoCloseFile(FILE* fp)
		: m_fp(fp)
	{

	}

	AutoCloseFile::~AutoCloseFile()
	{
		if (m_fp)
		{
			fclose(m_fp);
		}
	}
};
