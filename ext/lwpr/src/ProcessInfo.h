/*
 * $Id: ProcessInfo.h 1 2012-01-17 02:31:57Z  $
 */
#ifndef LWPR_PROCESS_INFO_H__
#define LWPR_PROCESS_INFO_H__
#include "LWPRType.h"
#include <map>
#include <vector>
#include <string>

namespace LWPR
{
	class ProcessInfo
	{
	public:
		typedef std::map< std::string, std::string > PStatusMap;
		static bool GetProcessInfoByPID(PStatusMap& status, PID_T pid);

		static bool EnumCurrentUserProcess(PIDList& pids);

		static int GetProcessFDCount(PID_T pid);

	private:
	};
};

#endif // end of LWPR_PROCESS_INFO_H__
