/*
 * $Id: GroupQueueImpl.h 40 2012-01-20 08:58:44Z  $
 */
#ifndef _LIBDB4Q_GROUP_QUEUE_IMPL_H__
#define _LIBDB4Q_GROUP_QUEUE_IMPL_H__
#include "db4q.h"
#include "MapedFileManager.h"

namespace DB4Q
{
	class GroupQueueImpl
	{
	public:
		GroupQueueImpl(const DB4QConfig* cfg = NULL);
		~GroupQueueImpl();

		INT32 enqueue(const TargetsRecordVector& records, const INT32 beginIndex);

		void commit();

		BOOL isAbleToRecover();

		INT32 recover(const GROUP_ID_VECTOR& gids);

		INT32 getGroupMessageCntTotal(const GROUP_ID& gid);

		INT32 getGroupMessageCntInMemory(const GROUP_ID& gid);

		INT32 getGroupMessageCntInDisk(const GROUP_ID& gid);

		BOOL isAbleToDeliver(const GROUP_ID& gid);

		BOOL dequeue(MSG_OFFSET_VECTOR& offsets, const GROUP_ID& gid, const INT32& max);

		void release(const MSG_OFFSET& offset);

		void hold(const MSG_OFFSET& offset);

	private:
		// No copying allowed
		GroupQueueImpl(const GroupQueueImpl&);
		void operator=(const GroupQueueImpl&);

	private:
		MapedFileManager m_MapedFileManager;

		INT32 m_pagesize;
	};
}
#endif // end of _LIBDB4Q_GROUP_QUEUE_IMPL_H__
