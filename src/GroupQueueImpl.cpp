/*
 * $Id: GroupQueueImpl.cpp 41 2012-03-12 07:51:28Z  $
 */
#include "GroupQueueImpl.h"
#include <unistd.h>
#include <sys/mman.h>

namespace DB4Q
{
	const INT32 TARGETS_RECORD_MAGIC_CODE = 1880681586;
	const UINT32 MAPED_FILE_SIZE_TMP2 = 67108864; // 64M

	GroupQueueImpl::GroupQueueImpl(const DB4QConfig* cfg /*= NULL*/)
	{
		m_pagesize = ::sysconf(_SC_PAGESIZE);
	}

	GroupQueueImpl::~GroupQueueImpl()
	{
	}

	//
	// 存储格式 <MAGIC_CODE> <MSG_OFFSET> <MSG_LEN> <GID_CNT> <GIDS...>
	//
	INT32 GroupQueueImpl::enqueue(const TargetsRecordVector& records, const INT32 beginIndex)
	{
		INT32 storeCnt = 0;
		INT32 refCnt = 0;

		if(records.empty())
		{
			return 0;
		}

		MapedFile mapedFile;
		UINT32 writingOffset = mapedFile.write_offset;
		if(!this->m_MapedFileManager.getCurrentMapedFile(mapedFile, records[0].size() + sizeof(TARGETS_RECORD_MAGIC_CODE)))
		{
			return -1;
		}

		for(size_t i = beginIndex; i < records.size(); i++)
		{
			// MAGIC_CODE
			memcpy((char*)mapedFile.pmap + writingOffset, &TARGETS_RECORD_MAGIC_CODE, sizeof(TARGETS_RECORD_MAGIC_CODE));
			writingOffset += sizeof(TARGETS_RECORD_MAGIC_CODE);

			// MSG_OFFSET
			memcpy((char*)mapedFile.pmap + writingOffset, &records[i].msg_index.offset, sizeof(MSG_OFFSET));
			writingOffset += sizeof(MSG_OFFSET);

			// MSG_LEN
			memcpy((char*)mapedFile.pmap + writingOffset, &records[i].msg_index.len, sizeof(UINT32));
			writingOffset += sizeof(UINT32);

			// GID_CNT
			GROUP_ID gid_cnt = records[i].gids.size();
			memcpy((char*)mapedFile.pmap + writingOffset, &gid_cnt, sizeof(gid_cnt));
			writingOffset += sizeof(gid_cnt);

			// GIDS...
			for(size_t k = 0; k < (size_t)gid_cnt; k++)
			{
				memcpy((char*)mapedFile.pmap + writingOffset, &records[i].gids[k], sizeof(GROUP_ID));
				writingOffset += sizeof(GROUP_ID);
			}

			storeCnt++;
			refCnt += gid_cnt;

			if((i + 1) < records.size())
			{
				if((records[i + 1].size() + sizeof(TARGETS_RECORD_MAGIC_CODE)) > (UINT32)(MAPED_FILE_SIZE_TMP2 - writingOffset))
				{
					break;
				}
			}
		}

		// 更新内存独立队列

#if 0
		//
		// TODO: 是否需要异步刷盘或者同步刷盘?
		//
		UINT32 beginOffset = mapedFile.write_offset;
		UINT32 offset = (beginOffset % m_pagesize);

		int ret = ::msync((char*)mapedFile.pmap + beginOffset - offset
		                  , writingOffset - beginOffset + offset
		                  , MS_ASYNC);
		assert(0 == ret);

		ret = ret;
#endif

		this->m_MapedFileManager.commitAndHold(writingOffset, refCnt);

		return storeCnt;
	}
}
