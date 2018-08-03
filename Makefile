
include $(ROOT)/Makefile.define

# 需要建立的目录
PATHS		= bin lib include build \
			  bin/log bin/config

# 项目中需要用第三方库
THIRDLIBS 	= libprotobuf.so libleveldb.so libcurl.so 

# 定义工程中的所有项目
LIBUVSERVER			= $(ROOT)/src

# 定义工程
SOLUTION	= $(LIBUVSERVER)  #$(CLUSTERD)

.PHONY: all install uninstall release clean test $(SOLUTION)

PATHS	  := $(addprefix $(ROOT)/, $(PATHS))

all : $(SOLUTION)
generate : $(SOLUTION)
test : $(SOLUTION)
clean : $(SOLUTION)

install : $(PATHS) $(THIRDLIBS)
	$(COPY) -a $(ROOT)/config/* $(ROOT)/bin/config/
	$(RM) $(RMFLAGS) $(ROOT)/bin/lib; $(LINK) -s $(ROOT)/lib $(ROOT)/bin/lib
	$(RM) $(RMFLAGS) $(ROOT)/bin/scripts; $(LINK) -s $(ROOT)/scripts $(ROOT)/bin/scripts

uninstall :
	$(RM) $(RMFLAGS) $(ROOT)/bin/scripts
	$(RM) $(RMFLAGS) $(PATHS) $(PLATFORM_FILE)

$(PATHS) :
	$(MKDIR) -p $@

$(SOLUTION):
	$(MAKE) -C $@ $(MAKECMDGOALS)

$(THIRDLIBS) : lib% :
	$(COPY) $(shell /sbin/ldconfig -p | grep $@ | cut -d '>' -f 2 | head -1)  $(ROOT)/lib/