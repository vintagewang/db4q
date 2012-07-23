#
# $Id: liblwpr.mak 1 2012-01-17 02:31:57Z  $
#
TOPDIR  = ..

APPNAME = lwpr

APPOBJS = \
		AtomicBoolean.o \
		AtomicInteger.o \
		Buffer.o \
		InputStream.o \
		IPCMSQ.o \
		IPCSEM.o \
		IPCSHM.o \
		Logger.o \
		Object.o \
		OutputStream.o \
		Socket.o \
		StringUtil.o \
		Synchronized.o \
		Utility.o \
		CFile.o \
		FileUtil.o \
		StringBuffer.o \
		ProgramOption.o \
		ProcessUtil.o \
		Thread.o \
		RWMutex.o \
		Mutex.o \
		Semph.o \
		EventMsqDispatch.o \
		SimpleProcessPool.o \
		MTNetServer.o \
		STNetServer.o \
		NetServer.o \
		Resource.o \
		ThreadPool.o \
		TimerThread.o \
		ConfigProperty.o \
		ThreadLocal.o \
		ProcessInfo.o \

APP_CMPFLGS =

APP_LDDFLGS =

# app build type: bldexe blddll bldslb bldexe_c blddll_c bldslb_c
APP_BUILDTYPE = blddll
all: $(APP_BUILDTYPE)
clean: cleanup

bldexe blddll bldslb bldexe_c blddll_c bldslb_c cleanup:
	@TOPDIR="$(TOPDIR)"; \
	APPNAME="$(APPNAME)"; \
	APPOBJS="$(APPOBJS)"; \
	APP_CMPFLGS="$(APP_CMPFLGS)"; \
	APP_LDDFLGS="$(APP_LDDFLGS)"; \
	APP_BUILDTYPE="$(APP_BUILDTYPE)"; \
	export TOPDIR APPNAME APPOBJS APP_CMPFLGS APP_LDDFLGS APP_BUILDTYPE; \
	$(MAKE) -f $(TOPDIR)/project/makstand $@
