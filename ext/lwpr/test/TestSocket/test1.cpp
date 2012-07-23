/*
 * $Id: test1.cpp 1 2012-01-17 02:31:57Z  $
 */
/*
 * 测试LWPR::Socket类是否能正常工作
 * 充当Server
 */
#include "Socket.h"
#include <iostream>
#include <stdio.h>

#include <sys/select.h>
#include <sys/time.h>
#include <unistd.h>

using namespace std;

int main(int argc, char *argv[])
{
	LWPR::SOCKET_FD_T nLinstenFd = LWPR::Socket::CreateServerTCPSocket(22222);
	if (nLinstenFd > 0)
	{
		cout << "CreateServerTCPSocket OK " << nLinstenFd << endl;
	}

	LWPR::SOCKET_FD_T nConnFd = LWPR::Socket::ConnectRemoteHost("127.0.0.1", 22222);
	if (nConnFd > 0)
	{
		cout << "ConnectRemoteHost OK " << nConnFd << endl;
	}
	//LWPR::Socket::CloseSocket(nLinstenFd);

	sleep(1000000);
	return 0;
}




