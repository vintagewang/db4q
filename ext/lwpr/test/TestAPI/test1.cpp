/*
 * $Id: test1.cpp 1 2012-01-17 02:31:57Z  $
 */
/*
 * 测试各种接口的调用
 */
#include "Socket.h"
#include "ProcessUtil.h"
#include "StringUtil.h"
#include "FileUtil.h"
#include <iostream>
#include <stdio.h>
#include <vector>
#include <string>

using namespace std;

int main(int argc, char *argv[])
{
	try
	{
		std::vector<std::string> cmd;
		LWPR::ProcessUtil::GetProcessCmdline(cmd);

		for(size_t i = 0; i < cmd.size(); i++)
		{
			printf("%5d [%s]\n", i, cmd[i].c_str());
		}
	}
	catch(const LWPR::Exception& e)
	{
		fprintf(stderr, "%s\n", e.what());
	}

	return 0;
}
