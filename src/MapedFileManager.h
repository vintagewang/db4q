/*
 * $Id: MapedFileManager.h 26 2012-01-19 02:47:09Z  $
 */
#ifndef _LIBDB4Q_MAPED_FILE_MANAGER_H__
#define _LIBDB4Q_MAPED_FILE_MANAGER_H__
#include "db4q.h"
#include <lwpr.h>

namespace DB4Q
{
	class MapedFileManager
	{
	public:
		MapedFileManager();
		~MapedFileManager();

		bool getCurrentMapedFile(MapedFile& mapedfile, const INT32& len);

		bool pickMapedFile(MapedFile& mapedfile, MSG_OFFSET& currentMapedKey, const MSG_OFFSET& offset);

		void commitAndHold(const UINT32& wroteOffset, const INT32& cnt);

		void hold(const MSG_OFFSET& offset, const INT32& cnt);

		void release(const MSG_OFFSET& offset, const INT32& cnt, const MSG_OFFSET& fetchingOffset);

		bool isEqualToCurrentMapedKey(const MSG_OFFSET& offset);

		MSG_OFFSET getKeyByOffset(const MSG_OFFSET& offset);

	private:
		bool mountMapedFile(const MSG_OFFSET& offset, MapedFile& mapedfile);
		void unmountMapedFile();
		void removeMapedFile();

		void getFileNameByOffset(std::string& filename, const MSG_OFFSET& offset);

	private:
		LWPR::Mutex m_mutex;
		std::map<MSG_OFFSET, MapedFile> m_MapedFiles;
		std::map<MSG_OFFSET, MapedFile> m_MapedFilesWillRemove;
		MSG_OFFSET m_currentMapedKey;
		volatile INT32 m_currentMapedCnt;
		volatile INT32 m_openFileTotal;
	};
}
#endif // end of _LIBDB4Q_MAPED_FILE_MANAGER_H__
