/*
 * $Id: db4q.h 39 2012-01-20 03:46:33Z  $
 */
#ifndef _LIBDB4Q_DB4Q_H__
#define _LIBDB4Q_DB4Q_H__
#include <vector>
#include <map>
#include <string>

namespace DB4Q
{
	typedef bool                         BOOL;      // bool
	typedef unsigned    char            UINT8;      // Unsigned  8 bit quantity
	typedef signed      char             INT8;      // Signed    8 bit quantity
	typedef unsigned    short           UINT16;     // Unsigned 16 bit quantity
	typedef signed      short            INT16;     // Signed   16 bit quantity
	typedef unsigned    int             UINT32;     // Unsigned 32 bit quantity
	typedef signed      int              INT32;     // Signed   32 bit quantity
	//typedef unsigned    long long       UINT64;     // Unsigned 64 bit quantity
	//typedef signed      long long        INT64;     // Signed   64 bit quantity

	// 64位
	typedef unsigned     long       UINT64;     // Unsigned 64 bit quantity
	typedef signed       long        INT64;     // Signed   64 bit quantity

	typedef float                        FP32;      // Single precision floating point
	typedef double                       FP64;      // Double precision floating point

	typedef UINT64 MSG_OFFSET;
	typedef std::vector<MSG_OFFSET> MSG_OFFSET_VECTOR;
	typedef std::map<std::string, std::string> DB4QConfig;

	class MessageStoreImpl;

	struct MsgData
	{
		void* msg;
		UINT32 len;
	};
	typedef MsgData MsgData;
	typedef std::vector<MsgData> MsgDataVector;

	struct MsgIndex
	{
		MSG_OFFSET offset;
		UINT32 len;

		MsgIndex();

		INT32 size() const;
	};
	typedef MsgIndex MsgIndex;
	typedef std::vector<MsgIndex> MsgIndexVector;

	struct MapedFile
	{
		int fd;
		void* pmap;
		UINT32 write_offset;
		INT32 ref_cnt;

		MapedFile()
			: pmap(NULL)
			, write_offset(0)
			, ref_cnt(0)
		{
		}
	};

	class MessageStore
	{
	public:
		MessageStore(const DB4QConfig* cfg = NULL);
		virtual ~MessageStore();

		/**
		 * 返回一次写入的消息数量，-1表示出错
		 */
		INT32 put(const MsgDataVector& msgs, const INT32 beginIndex, MapedFile& mapedFile, UINT32& wroteOffset);

		void commitPutAction(const MapedFile& mapedFile, const UINT32& wroteOffset, const BOOL& force);

		void fetch(MsgIndexVector& msgIndexs, MSG_OFFSET& fetchedOffset, const INT32& max);

		void commitFetchAction(const MSG_OFFSET& fetchedOffset);

		void release(const MSG_OFFSET& offset, const INT32& cnt);

		void hold(const MSG_OFFSET& offset, const INT32& cnt);

	private:
		// No copying allowed
		MessageStore(const MessageStore&);
		void operator=(const MessageStore&);

		MessageStoreImpl* m_pImpl;
	};

	typedef UINT16 GROUP_ID;
	typedef std::vector<GROUP_ID> GROUP_ID_VECTOR;
	typedef INT32 MAGIC_CODE;

	struct TargetsRecord
	{
		MsgIndex msg_index;
		GROUP_ID_VECTOR gids;

		TargetsRecord();
		INT32 size() const;
	};

	typedef struct TargetsRecord TargetsRecord;
	typedef std::vector<TargetsRecord> TargetsRecordVector;

	class GroupQueueImpl;

	class GroupQueue
	{
	public:
		GroupQueue(const DB4QConfig* cfg = NULL);
		virtual ~GroupQueue();

		/**
		* 返回一次写入的消息索引数量，-1表示出错
		*/
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
		GroupQueue(const GroupQueue&);
		void operator=(const GroupQueue&);

		GroupQueueImpl* m_pImpl;
	};
}
#endif // end of _LIBDB4Q_DB4Q_H__
