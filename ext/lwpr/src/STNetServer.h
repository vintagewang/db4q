/*
 * $Id: STNetServer.h 1 2012-01-17 02:31:57Z  $
 */
#ifndef LWPR_STNET_SERVER_H__
#define LWPR_STNET_SERVER_H__
#include "LWPRType.h"
#include "NetServer.h"
#include <vector>
#include <map>

namespace LWPR
{
	class STNetServer:  public NetServer
	{
		typedef std::vector< SOCKET_FD_T > SocketVector;
		typedef std::map< SOCKET_FD_T, time_t > SocketMap;

	public:

		STNetServer(const NET_SERVER_OPTION_T& opt);
		~STNetServer();

		virtual void ActivateService();

		virtual void Shutdown();

		virtual void Go();

		virtual void EnableSocket(SOCKET_FD_T fd);

		virtual void WriteSocketPositive(SOCKET_FD_T fd, char *buf, int length);

		virtual void WriteSocketPositive(const SOCKETFDList& fds, char *buf, int length);

		virtual bool IsServiceBusy();

		virtual std::string GetListenAddr();

		virtual void DestroySocket(SOCKET_FD_T fd);

		virtual LWPR::INT32 ClientTotal();

	private:
		bool				m_bStoped;			// 是否已经停止运行
		SOCKET_FD_T			m_fdListen;			// 监听套接口
		SocketMap			m_mapSocketTime;	// 当前所有活的SOCKET的最后活跃时间
		NET_SERVER_OPTION_T	m_NetServerOption;	// 选项配置
		std::string			m_strListenAddr;
		AtomicInteger		m_nTotalClient;		// 客户端总数
	};
}
#endif // end of QB_STNET_SERVER_H__
