/*
 * $Id: MapedFileManager.cpp 41 2012-03-12 07:51:28Z  $
 */
#include "MapedFileManager.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>

namespace DB4Q
{
	const UINT32 MAPED_FILE_SIZE = 67108864; // 64M
	const INT32 MAPED_FILE_CNT = 8;
	const char* const MAPED_FILE_DIR = "./db4q/store";

	MapedFileManager::MapedFileManager()
		: m_currentMapedKey(0)
		, m_currentMapedCnt(0)
		, m_openFileTotal(0)
	{
	}

	MapedFileManager::~MapedFileManager()
	{
	}

	bool MapedFileManager::getCurrentMapedFile(MapedFile & mapedfile, const INT32 & len)
	{
		LWPR::Synchronized sync(m_mutex);

		if(m_MapedFiles.empty())
		{
			return mountMapedFile(0, mapedfile);
		}
		else
		{
			mapedfile = m_MapedFiles[m_currentMapedKey];

			// 当前文件已经写满
			if((mapedfile.write_offset + len) > MAPED_FILE_SIZE)
			{
				// TODO: 初始化文件末尾没有有效利用的空间，防止OS CRASH恢复出错
				return mountMapedFile(m_currentMapedKey + MAPED_FILE_SIZE, mapedfile);
			}
		}

		return true;
	}

	bool MapedFileManager::mountMapedFile(const MSG_OFFSET & offset, MapedFile & mapedfile)
	{
		unmountMapedFile();

		std::string filePath;
		getFileNameByOffset(filePath, offset);

		memset(&mapedfile, 0, sizeof(mapedfile));

		mapedfile.fd = ::open(filePath.c_str(), O_RDWR | O_CREAT);
		if(-1 == mapedfile.fd)
		{
			fprintf(stderr, "create file [%s] error, [%d] \n", filePath.c_str(), errno);
			return false;
		}

		// ftruncate 不会真实的写磁盘， 分配的文件大小在系统掉电重启后可能失效
		if(-1 == ftruncate(mapedfile.fd, MAPED_FILE_SIZE))
		{
			fprintf(stderr, "ftruncate file [%s] error, [%d] \n", filePath.c_str(), errno);
			close(mapedfile.fd);
			return false;
		}

		// TODO: 是否需要这样处理?
#if 1
		// 抢占文件磁盘空间
		char bufTmp[16] = {0};
		memset(bufTmp, 'T', sizeof(bufTmp));
		ssize_t ret = pwrite(mapedfile.fd, bufTmp, sizeof(bufTmp), MAPED_FILE_SIZE - sizeof(bufTmp));
		if(sizeof(bufTmp) != ret)
		{
			fprintf(stderr, "pwrite file [%s] error, [%d] \n", filePath.c_str(), errno);
			close(mapedfile.fd);
			return false;
		}
#endif

		mapedfile.pmap = ::mmap(NULL, MAPED_FILE_SIZE, PROT_WRITE, MAP_SHARED, mapedfile.fd, 0);
		if(NULL == mapedfile.pmap)
		{
			fprintf(stderr, "mmap file [%s] error, [%d] \n", filePath.c_str(), errno);
			close(mapedfile.fd);
			return false;
		}

		m_currentMapedKey = offset;
		m_currentMapedCnt++;
		m_openFileTotal++;
		m_MapedFiles[m_currentMapedKey] = mapedfile;

		return true;
	}

	void MapedFileManager::unmountMapedFile()
	{
		void *start = NULL;
		MSG_OFFSET offsetKey = 0;

		if(m_currentMapedCnt >= MAPED_FILE_CNT)
		{
			std::map<MSG_OFFSET, MapedFile>::reverse_iterator it = m_MapedFiles.rbegin();
			for(int i = 0; it != m_MapedFiles.rend() && i < MAPED_FILE_CNT; i++, it++)
			{
				if((MAPED_FILE_CNT - 1) == i)
				{
					offsetKey = it->first;
					start = it->second.pmap;
					it->second.pmap = NULL;
					m_currentMapedCnt--;
				}
			}
		}

		if(start)
		{
			std::string filePathName;
			getFileNameByOffset(filePathName, offsetKey);
			int ret = ::munmap(start, MAPED_FILE_SIZE);
			fprintf(stdout, "[%s] unmap [%s]\n", filePathName.c_str(), (0 == ret) ? "OK" : "Failed");
			assert(0 == ret);
		}
	}

	void MapedFileManager::removeMapedFile()
	{
		if(!m_MapedFilesWillRemove.empty())
		{
			std::map<MSG_OFFSET, MapedFile>::iterator it = m_MapedFiles.begin();
			for(; it != m_MapedFiles.end(); it++)
			{
				std::string filePathName;
				getFileNameByOffset(filePathName, it->first);

				if(it->second.pmap)
				{
					::munmap(it->second.pmap, MAPED_FILE_SIZE);
				}

				close(it->second.fd);

				m_openFileTotal--;

				int ret = ::remove(filePathName.c_str());
				fprintf(stdout, "[%s] remove [%s]\n", filePathName.c_str(), (0 == ret) ? "OK" : "Failed");
			}

			m_MapedFilesWillRemove.clear();
		}
	}

	DB4Q::MSG_OFFSET MapedFileManager::getKeyByOffset(const MSG_OFFSET& offset)
	{
		ldiv_t res = ldiv(offset, MAPED_FILE_SIZE);
		return res.quot;
	}

	void MapedFileManager::getFileNameByOffset(std::string& filename, const MSG_OFFSET& offset)
	{
		// TODO: 长度需要修改 %s/%020lu 在32位系统要测试
		char bufName[128] = {0};
		sprintf(bufName, "%s/%020lu", MAPED_FILE_DIR, offset);
		filename = bufName;
	}

	bool MapedFileManager::pickMapedFile(MapedFile& mapedfile, MSG_OFFSET& currentMapedKey, const MSG_OFFSET& offset)
	{
		MSG_OFFSET key = getKeyByOffset(offset);

		LWPR::Synchronized syn(m_mutex);

		std::map<MSG_OFFSET, MapedFile>::iterator it = m_MapedFiles.find(key);
		if(it != m_MapedFiles.end())
		{
			mapedfile = it->second;
			currentMapedKey = this->m_currentMapedKey;
			return true;
		}

		return false;
	}

	void MapedFileManager::commitAndHold(const UINT32& wroteOffset, const INT32& cnt)
	{
		LWPR::Synchronized syn(m_mutex);

		std::map<MSG_OFFSET, MapedFile>::iterator it = m_MapedFiles.find(m_currentMapedKey);
		if(it != m_MapedFiles.end())
		{
			it->second.write_offset = wroteOffset;
			it->second.ref_cnt += cnt;
		}
	}

	void MapedFileManager::hold(const MSG_OFFSET& offset, const INT32& cnt)
	{
		MSG_OFFSET key = getKeyByOffset(offset);

		LWPR::Synchronized syn(m_mutex);

		std::map<MSG_OFFSET, MapedFile>::iterator it = m_MapedFiles.find(key);
		if(it != m_MapedFiles.end())
		{
			it->second.ref_cnt += cnt;
		}
	}

	void MapedFileManager::release(const MSG_OFFSET& offset, const INT32& cnt, const MSG_OFFSET& fetchingOffset)
	{
		MSG_OFFSET key = getKeyByOffset(offset);

		LWPR::Synchronized syn(m_mutex);

		std::map<MSG_OFFSET, MapedFile>::iterator it = m_MapedFiles.find(key);
		if(it != m_MapedFiles.end())
		{
			it->second.ref_cnt -= cnt;

			bool rm = (m_currentMapedKey != key
			           && fetchingOffset >= (key + MAPED_FILE_SIZE)
			           && (it->second.ref_cnt <= 0));
			if(rm)
			{
				m_MapedFilesWillRemove[it->first] = it->second;
				m_MapedFiles.erase(it);
			}
		}
	}

	bool MapedFileManager::isEqualToCurrentMapedKey(const MSG_OFFSET& offset)
	{
		return (this->getKeyByOffset(offset) == m_currentMapedKey);
	}
}
