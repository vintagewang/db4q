#
# $Id: db4q.mak 15 2012-01-18 01:35:34Z  $
#
TOPDIR  = ..

APPNAME = db4q

APPOBJS = \
		db4q.o \
		GroupQueueImpl.o \
		MessageStoreImpl.o \
		MapedFileManager.o \

APP_CMPFLGS  =

APP_LDDFLGS  = -llwpr

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
