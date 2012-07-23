/*
 * $Id: PmonCmdline.cpp 1 2012-01-17 02:31:57Z  $
 */
#include "PmonCmdline.h"


PmonCmdline::PmonCmdline()
{
}

PmonCmdline* PmonCmdline::GetInstance()
{
	static PmonCmdline *singleton = new PmonCmdline();
	return singleton;
}

bool PmonCmdline::CheckCmdline()
{
	bool bRetResult = true;

	return bRetResult;
}

void PmonCmdline::GetPIDS(LWPR::PIDList& pids)
{
	for(size_t i = 0; i < m_OtherArguments.size(); i++)
	{
		pids.push_back(atoi(m_OtherArguments[i].c_str()));
	}
}

int PmonCmdline::GetStatisticsInterval()
{
	std::vector< std::string > args;

	if(GetArgumentsByOption('d', args))
	{
		return atoi(args[0].c_str());
	}

	return 3;
}

std::string PmonCmdline::GetLogDir()
{
	std::vector< std::string > args;

	if(GetArgumentsByOption('d', args))
	{
		return args[0].c_str();
	}

	return LWPR::ProcessUtil::GetWorkingDir();
}
