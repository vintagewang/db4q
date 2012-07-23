/*
 * $Id: MessageStoreImpl.cpp 39 2012-01-20 03:46:33Z  $
 */
#include "MessageStoreImpl.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>

// >0 表示向前对齐， 0 表示向后对齐
#define MAPED_FILE_COMMIT_TYPE_FORWARD_ALIGN	1


namespace DB4Q
{
	const INT32 MSG_BLOCK_MAGIC_CODE = 1880681586;
	const INT32 EMPTY_BLOCK_MAGIC_CODE = 1880681586 + 1;
	const UINT32 MAPED_FILE_SIZE_TMP1 = 67108864; // 64M

	MessageStoreImpl::MessageStoreImpl(const DB4QConfig* cfg /*= NULL*/)
		: m_fetchingOffset(0)
	{
		m_pagesize = ::sysconf(_SC_PAGESIZE);
	}

	MessageStoreImpl::MessageStoreImpl(const MessageStoreImpl&)
	{
		assert(0);
	}

	MessageStoreImpl::~MessageStoreImpl()
	{

	}

	void MessageStoreImpl::operator=(const MessageStoreImpl&)
	{
		assert(0);
	}

	// 存储格式 <MAGIC CODE> <LEN> <DATA>
	INT32 MessageStoreImpl::put(const MsgDataVector& msgs, const INT32 beginIndex, MapedFile& mapedFile, UINT32& wroteOffset)
	{
		INT32 storeCnt = 0;

		if(msgs.empty())
		{
			return -1;
		}

		if(!this->m_MapedFileManager.getCurrentMapedFile(mapedFile, getMsgLen(msgs[beginIndex].len)))
		{
			return -1;
		}

		UINT32 writingOffset = mapedFile.write_offset;
		for(size_t i = beginIndex; i < msgs.size(); i++)
		{
			INT32 len = msgs[i].len;
			void* msg = msgs[i].msg;

			memcpy((char*)mapedFile.pmap + writingOffset, &MSG_BLOCK_MAGIC_CODE, sizeof(MSG_BLOCK_MAGIC_CODE));
			writingOffset += sizeof(MSG_BLOCK_MAGIC_CODE);

			memcpy((char*)mapedFile.pmap + writingOffset, &len, sizeof(len));
			writingOffset += sizeof(len);

			memcpy((char*)mapedFile.pmap + writingOffset, msg, len);
			writingOffset += len;

			storeCnt++;

			if((i + 1) < msgs.size())
			{
				if((this->getMsgLen(msgs[i + 1].len) > (UINT32)(MAPED_FILE_SIZE_TMP1 - writingOffset)))
				{
					break;
				}
			}
		}

		wroteOffset = writingOffset;

		return storeCnt;
	}

	void MessageStoreImpl::commitPutAction(const MapedFile& mapedFile, const UINT32& wroteOffset, const BOOL& force)
	{
		//
		// 两种方式比较：在自己虚拟机上，第二种要比第一种性能高出30%
		// 在公司性能环境虚拟机上，两种效果一样，why?
		// 物理机上效果也一样
		//
#if MAPED_FILE_COMMIT_TYPE_FORWARD_ALIGN
		if(force)
		{
			UINT32 beginOffset = mapedFile.write_offset;
			UINT32 offset = (beginOffset % m_pagesize);

			int ret = ::msync((char*)mapedFile.pmap + beginOffset - offset
			                  , wroteOffset - beginOffset + offset
			                  , MS_SYNC);
			assert(0 == ret);

			ret = ret;
		}

		this->m_MapedFileManager.commitAndHold(wroteOffset, 0);
#else
		UINT32 offset = 0;

		if(force)
		{
			UINT32 beginOffset = mapedFile.write_offset;

			offset = (wroteOffset % m_pagesize);
			if(offset > 0)
			{
				offset = m_pagesize - offset;
				fillEmptyBlock((char*)mapedFile.pmap + wroteOffset, offset);
			}

			// TODO: 此处可能需要优化
			int ret = ::msync((char*)mapedFile.pmap + beginOffset
			                  , wroteOffset + offset - beginOffset
			                  , MS_SYNC);

			assert(0 == ret);
			ret = ret;
		}

		this->m_MapedFileManager.commitAndHold(wroteOffset + offset, 0);
#endif
	}

	void MessageStoreImpl::fetch(MsgIndexVector& msgIndexs, MSG_OFFSET& fetchedOffset, const INT32& max)
	{
		INT32 scanedMsgCnt = 0;
		MSG_OFFSET currentFetching = (MSG_OFFSET)m_fetchingOffset;

		const UINT32 minMsgLen = getMsgLen(0);

		while(scanedMsgCnt < max)
		{
			MapedFile mapedFile;
			MSG_OFFSET currentMapedKey = 0;
			if(!this->m_MapedFileManager.pickMapedFile(mapedFile, currentMapedKey, currentFetching))
			{
				break;
			}

			bool currentMapedFileOver = false;

			// 数据在mmap memory buffer
			if(mapedFile.pmap != NULL)
			{
				UINT32 offset = currentFetching % MAPED_FILE_SIZE_TMP1;

				for(; (offset < mapedFile.write_offset) && (scanedMsgCnt < max);)
				{
					UINT32 lastOffset = offset;

					if((mapedFile.write_offset - offset) <= minMsgLen)
					{
						offset = mapedFile.write_offset;
					}
					else
					{
						INT32 magicCode = 0;
						INT32 msgLen = 0;

						memcpy(&magicCode, (char*)mapedFile.pmap + offset, sizeof(magicCode));

						memcpy(&msgLen, (char*)mapedFile.pmap + offset + sizeof(magicCode), sizeof(msgLen));

						// 有效Msg
						if(MSG_BLOCK_MAGIC_CODE == magicCode)
						{
							MsgIndex msgIndex;
							msgIndex.offset = lastOffset + getMsgLen(0);
							msgIndex.len = msgLen;
							msgIndexs.push_back(msgIndex);

							offset += getMsgLen(msgLen);
							scanedMsgCnt++;
						}
#if MAPED_FILE_COMMIT_TYPE_FORWARD_ALIGN
						else
						{
							offset = mapedFile.write_offset;
						}
#else
						// 长度>=8字节的空洞
						else if(EMPTY_BLOCK_MAGIC_CODE == magicCode)
						{
							offset += getMsgLen(msgLen);
						}
						// 长度<8字节的空洞
						else
						{
							while(0 == *((char*)mapedFile.pmap + offset)) offset++;
						}
#endif
					}
				}

				currentMapedFileOver = offset >= mapedFile.write_offset;
				currentFetching += (offset - (currentFetching % MAPED_FILE_SIZE_TMP1));
			}
			// 数据在文件中
			else
			{
				// TODO: 增加对文件的处理
				assert(0);
			}

			// 转移至下一个文件块
			if(currentMapedFileOver
			   && (this->m_MapedFileManager.getKeyByOffset(currentFetching) != currentMapedKey))
			{
				currentFetching += (MAPED_FILE_SIZE_TMP1 - (currentFetching % MAPED_FILE_SIZE_TMP1));
			}
			else
			{
				break;
			}
		}

		fetchedOffset = currentFetching;
	}

	void MessageStoreImpl::commitFetchAction(const MSG_OFFSET& fetchedOffset)
	{
		// TODO: 以后考虑使用原子操作
		this->m_fetchingOffset = fetchedOffset;
	}

	void MessageStoreImpl::release(const MSG_OFFSET & offset, const INT32 & cnt)
	{
		this->m_MapedFileManager.release(offset, cnt, (MSG_OFFSET)m_fetchingOffset);
	}

	void MessageStoreImpl::hold(const MSG_OFFSET & offset, const INT32 & cnt)
	{
		this->m_MapedFileManager.hold(offset, cnt);
	}

	UINT32 MessageStoreImpl::getMsgLen(UINT32 len)
	{
		return (sizeof(UINT32) + len + sizeof(UINT32));
	}

	// 存储格式 <MAGIC CODE> <LEN> <0000>
	void MessageStoreImpl::fillEmptyBlock(void* pMapBegin, const UINT32& size)
	{
		assert(NULL != pMapBegin);

		if(size >= getMsgLen(0))
		{
			UINT32 writingOffset = 0;
			INT32 len = (size - getMsgLen(0));

			memcpy((char*)pMapBegin + writingOffset, &EMPTY_BLOCK_MAGIC_CODE, sizeof(EMPTY_BLOCK_MAGIC_CODE));
			writingOffset += sizeof(EMPTY_BLOCK_MAGIC_CODE);

			memcpy((char*)pMapBegin + writingOffset, &len, sizeof(len));
			writingOffset += sizeof(len);

			writingOffset += len;
		}
		else
		{
			memset(pMapBegin, 0, size);
		}
	}
}
