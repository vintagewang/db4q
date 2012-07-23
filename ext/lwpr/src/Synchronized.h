/*
 * $Id: Synchronized.h 1 2012-01-17 02:31:57Z  $
 */
#ifndef LWPR_SYNCHRONIZED_H__
#define LWPR_SYNCHRONIZED_H__
#include "LWPRType.h"
#include "IPCSEM.h"
#include "Mutex.h"
#include "RWMutex.h"
#include "Semph.h"
#include "Object.h"
#include "Var.h"

namespace LWPR
{
	class Synchronized : public LWPR::Object
	{
		typedef enum syn_type_e
		{
			SYN_IPCSEM,
			SYN_MUTEX,
			SYN_SEMPH,
		} SYN_TYPE_E;

	public:
		Synchronized(const IPCID_T id);
		Synchronized(Mutex &lock);
		Synchronized(Semph &lock);
		~Synchronized();

	private:
		SYN_TYPE_E m_synType;

		IPCID_T m_id;
		Mutex* m_pMutex;
		Semph* m_pSemph;
	};

	class SynchronizedRead : public LWPR::Object
	{
	public:
		SynchronizedRead(RWMutex &lock);
		~SynchronizedRead();

	private:
		RWMutex* m_pRWMutex;
	};

	class SynchronizedWrite : public LWPR::Object
	{
	public:
		SynchronizedWrite(RWMutex &lock);
		~SynchronizedWrite();

	private:
		RWMutex* m_pRWMutex;
	};

	DECLAREVAR(Synchronized);
	DECLAREVAR(SynchronizedRead);
	DECLAREVAR(SynchronizedWrite);
};

#endif // end of LWPR_SYNCHRONIZED_H__
