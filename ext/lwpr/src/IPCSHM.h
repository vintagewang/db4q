/*
 * $Id: IPCSHM.h 1 2012-01-17 02:31:57Z  $
 */
#ifndef LWPR_IPCSHM_H__
#define LWPR_IPCSHM_H__
#include "LWPRType.h"
#include "Exception.h"

namespace LWPR
{
	class IPCSHM
	{
	public:
		/**
		 * 创建一个共享内存对象
		 */
		static IPCID_T Create(const IPCKEY_T key,
		                      int size,
		                      IPC_CREATE_TYPE_E type = LWPR::CREATE_NEW_ONE,
		                      int perm = IPC_DEFAULT_PERM);

		/**
		 * 根据IPC Key返回相应的IPC ID
		 */
		static IPCID_T GetID(const IPCKEY_T key);

		/**
		 * 销毁一个共享内存对象
		 */
		static void Destroy(const IPCID_T id);

		/**
		 * 另内核将共享内存地址块挂载至本进程
		 */
		static void* Mount(const IPCID_T id, PERMISSION_E prms = LWPR::RW);

		/**
		 * 卸载本进程挂载的进程地址
		 */
		static void Unmount(void *addr);

		/**
		 * 根据MagicCode获取系统中可访问的共享内存ID
		 */
		static int FetchSHMID(IPCIDList& ids, LWPR::UINT32 magic_code);

	private:
	};

	DEFINE_EXCEPTION(LWPR_IPC_SHM_GET_ERR);
	DEFINE_EXCEPTION(LWPR_IPC_SHM_SIZE_ERR);
	DEFINE_EXCEPTION(LWPR_IPC_SHM_SHMAT_ERR);
	DEFINE_EXCEPTION(LWPR_IPC_SHM_SHMDT_ERR);

	DEFINE_EXCEPTION(LWPR_IPC_SHM_NOT_EXIST);
};

#endif // end of LWPR_IPCSHM_H__
