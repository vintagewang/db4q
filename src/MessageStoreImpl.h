/*
 * $Id: MessageStoreImpl.h 39 2012-01-20 03:46:33Z  $
 */
#ifndef _LIBDB4Q_MESSAGE_STORE_IMPL_H__
#define _LIBDB4Q_MESSAGE_STORE_IMPL_H__
#include "db4q.h"
#include "MapedFileManager.h"
#include <lwpr.h>

namespace DB4Q
{
	class MessageStoreImpl
	{
	public:
		MessageStoreImpl(const DB4QConfig* cfg = NULL);
		virtual ~MessageStoreImpl();

		INT32 put(const MsgDataVector& msgs, const INT32 beginIndex, MapedFile& mapedFile, UINT32& wroteOffset);

		void commitPutAction(const MapedFile& mapedFile, const UINT32& wroteOffset, const BOOL& force);

		void fetch(MsgIndexVector& msgIndexs, MSG_OFFSET& fetchedOffset, const INT32& max);

		void commitFetchAction(const MSG_OFFSET& fetchedOffset);

		void release(const MSG_OFFSET& offset, const INT32& cnt);

		void hold(const MSG_OFFSET& offset, const INT32& cnt);

	private:
		// No copying allowed
		MessageStoreImpl(const MessageStoreImpl&);
		void operator=(const MessageStoreImpl&);

		UINT32 getMsgLen(UINT32 len);

		void fillEmptyBlock(void* pMapBegin, const UINT32& size);

	private:
		MapedFileManager m_MapedFileManager;

		volatile MSG_OFFSET m_fetchingOffset;

		// config
		INT32 m_pagesize;
	};
}
#endif // end of _LIBDB4Q_MESSAGE_STORE_IMPL_H__
