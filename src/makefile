SHELL    = /bin/sh

TOPDIR  := $(shell pwd)/..


all clean:
	$(MAKE) -f db4q.mak $@
	@-mkdir $(TOPDIR)/include/DB4Q
	@-cp $(TOPDIR)/src/db4q.h $(TOPDIR)/include/DB4Q/
