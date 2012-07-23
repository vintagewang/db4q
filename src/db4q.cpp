/*
 * $Id: db4q.cpp 39 2012-01-20 03:46:33Z  $
 */
#include "db4q.h"
#include "MessageStoreImpl.h"
#include "GroupQueueImpl.h"

namespace DB4Q
{
	//-----------------------------------------------------------------------
	// class MessageStore
	//-----------------------------------------------------------------------
	MessageStore::MessageStore(const DB4QConfig* cfg /*= NULL*/)
		: m_pImpl(new MessageStoreImpl(cfg))
	{

	}

	MessageStore::MessageStore(const MessageStore&)
	{
	}

	void MessageStore::operator=(const MessageStore&)
	{

	}

	MessageStore::~MessageStore()
	{
		delete m_pImpl;
		m_pImpl = NULL;
	}

	INT32 MessageStore::put(const MsgDataVector& msgs, const INT32 beginIndex, MapedFile& mapedFile, UINT32& wroteOffset)
	{
		return this->m_pImpl->put(msgs, beginIndex, mapedFile, wroteOffset);
	}

	void MessageStore::commitPutAction(const MapedFile& mapedFile, const UINT32& wroteOffset, const BOOL& force)
	{
		this->m_pImpl->commitPutAction(mapedFile, wroteOffset, force);
	}

	void MessageStore::fetch(MsgIndexVector& msgIndexs, MSG_OFFSET& fetchedOffset, const INT32& max)
	{
		this->m_pImpl->fetch(msgIndexs, fetchedOffset, max);
	}

	void MessageStore::commitFetchAction(const MSG_OFFSET& fetchedOffset)
	{
		this->m_pImpl->commitFetchAction(fetchedOffset);
	}

	void MessageStore::release(const MSG_OFFSET& offset, const INT32& cnt)
	{
		this->m_pImpl->release(offset, cnt);
	}

	void MessageStore::hold(const MSG_OFFSET& offset, const INT32& cnt)
	{
		this->m_pImpl->hold(offset, cnt);
	}

	//-----------------------------------------------------------------------
	// struct MsgIndex
	//-----------------------------------------------------------------------
	MsgIndex::MsgIndex()
		: offset(0)
		, len(0)
	{
	}

	INT32 MsgIndex::size() const
	{
		return  sizeof(offset)
		        + sizeof(len);
	}

	//-----------------------------------------------------------------------
	// struct TargetsRecord
	//-----------------------------------------------------------------------
	TargetsRecord::TargetsRecord()
	{

	}

	INT32 TargetsRecord::size() const
	{
		return msg_index.size()
		       + sizeof(GROUP_ID)
		       + sizeof(GROUP_ID) * gids.size();
	}

	//-----------------------------------------------------------------------
	// class MessageStore
	//-----------------------------------------------------------------------
	GroupQueue::GroupQueue(const DB4QConfig* cfg /*= NULL*/)
		: m_pImpl(new GroupQueueImpl(cfg))
	{
	}

	GroupQueue::GroupQueue(const GroupQueue&)
	{
	}

	void GroupQueue::operator=(const GroupQueue&)
	{
	}

	GroupQueue::~GroupQueue()
	{
		delete m_pImpl;
		m_pImpl = NULL;
	}

	INT32 GroupQueue::enqueue(const TargetsRecordVector& records, const INT32 beginIndex)
	{
		return this->m_pImpl->enqueue(records, beginIndex);
	}

	void GroupQueue::commit()
	{

	}

	BOOL GroupQueue::isAbleToRecover()
	{
		return false;
	}

	INT32 GroupQueue::recover(const GROUP_ID_VECTOR& gids)
	{
		return 0;
	}

	INT32 GroupQueue::getGroupMessageCntTotal(const GROUP_ID& gid)
	{
		return 0;
	}

	INT32 GroupQueue::getGroupMessageCntInMemory(const GROUP_ID& gid)
	{
		return 0;
	}

	INT32 GroupQueue::getGroupMessageCntInDisk(const GROUP_ID& gid)
	{
		return 0;
	}

	BOOL GroupQueue::isAbleToDeliver(const GROUP_ID& gid)
	{
		return 0;
	}

	BOOL GroupQueue::dequeue(MSG_OFFSET_VECTOR& offsets, const GROUP_ID& gid, const INT32& max)
	{
		return 0;
	}

	void GroupQueue::release(const MSG_OFFSET& offset)
	{

	}

	void GroupQueue::hold(const MSG_OFFSET& offset)
	{

	}
}
