#
# $Id: makefile 28 2012-01-19 05:40:02Z  $
#

SHELL    = /bin/sh

TOPDIR  := $(shell pwd)/..

TARGETS_DIR  = \
		$(TOPDIR)/src \
		$(TOPDIR)/test/TEST_002 \

all::
	@cd $(TOPDIR)/ext/lwpr/project; $(MAKE)
	@-mkdir $(TOPDIR)/include/lwpr
	@-cp $(TOPDIR)/ext/lwpr/src/*.h $(TOPDIR)/include/lwpr/
	@-mv $(TOPDIR)/include/lwpr/lwpr.h $(TOPDIR)/include/
	@-cp $(TOPDIR)/ext/lwpr/lib/* $(TOPDIR)/lib/
	@-cp $(TOPDIR)/ext/lwpr/project/makstand $(TOPDIR)/project/
	@for tsubdir in $(TARGETS_DIR); \
	do \
		echo "making $${tsubdir}"; \
		( cd $${tsubdir} && $(MAKE) ) || exit 1; \
	done

clean::
	@cd $(TOPDIR)/ext/lwpr/project; $(MAKE) clean
	@-cp $(TOPDIR)/ext/lwpr/project/makstand $(TOPDIR)/project/
	@for tsubdir in $(TARGETS_DIR); \
	do \
		echo "clean $${tsubdir}"; \
		( cd $${tsubdir} && $(MAKE) clean ) || exit 1; \
	done
