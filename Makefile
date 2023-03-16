MAKEFILES = $(wildcard */Makefile)
.PHONY : all clean

all :
	@for MF in $(MAKEFILES); \
	do \
	    make -C `dirname $${MF}` ; \
	done;

clean :
	@for MF in $(MAKEFILES); \
	do \
	    make -C `dirname $${MF}` clean ; \
	done;
