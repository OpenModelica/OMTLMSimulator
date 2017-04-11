# Makefile for TLMManager & Plugin

ifeq ($(MAKEFILEHEADHOME),)
  UP=..
  MAKEFILEHEADHOME=$(UP)
  BINDIR=$(UP)/bin
else
  # This is for BEAST
  UP=$(MAKEFILEHEADHOME)/src
  BINDIR=$(UP)/../bin/$(ABI)
endif

include $(MAKEFILEHEADHOME)/Makefile.head
MISCHOME = ../extralibs/misc

SRCCLT= Plugin/PluginImplementer.cc \
	Plugin/MonitoringPluginImplementer.cc \
	Communication/TLMClientComm.cc \
	Communication/TLMCommUtil.cc \
	Interfaces/TLMInterface.cc \
	Interfaces/TLMInterfaceSignal.cc \
	Interfaces/TLMInterfaceSignalInput.cc \
	Interfaces/TLMInterfaceSignalOutput.cc \
	Interfaces/TLMInterface1D.cc \
	Interfaces/TLMInterface3D.cc \
	Parameters/ComponentParameter.cc \
	Logging/TLMErrorLog.cc \
	Plugin/TLMPlugin.cc 


SRCMGR= Communication/ManagerCommHandler.cc \
	ManagerMain.cc	\
	CompositeModels/CompositeModel.cc \
	CompositeModels/CompositeModelReader.cc \
	Communication/TLMCommUtil.cc \
	Communication/TLMManagerComm.cc \
	Communication/TLMMessageQueue.cc \
	Logging/TLMErrorLog.cc

SRCSRVLIB= Communication/ManagerCommHandler.cc \
	CompositeModels/CompositeModel.cc \
	Communication/TLMCommUtil.cc \
	Communication/TLMManagerComm.cc \
	Communication/TLMMessageQueue.cc \
	Logging/TLMErrorLog.cc

SRCMONITOR= $(SRCCLT) \
	CompositeModels/CompositeModel.cc \
	CompositeModels/CompositeModelReader.cc \
	MonitorMain.cc

CP=cp

SRC= $($(SRCTYPE))

SRCTST=	TLMTestApp.cc

OBJS = $(SRC:%.cc=$(ABI)/%.o)

INCLUDES= -I. \
	-I$(MISCHOME)/include \
	-I$(TIMEHOME) \
	$(INCLPTHREAD)

LIBS = 	-L$(MISCHOME)/lib/$(ABI) \
	-L$(TIMEHOME)/lib/$(ABI) \
	-ltlmmisc -lrtime

INCXTR=
LIBXTR=

default: help all

help:
	@echo Makefile for TLMPlugin related files.
	@echo Possible targets are:
	@echo lib - creates the libTLM.a and libTLM_m.a libraries - the client side of the plugin
	@echo manager - creates the tlmmanager application
	@echo all, defaulf: build everything.


all: lib manager monitor test

lib: lib_s
	$(MAKE) dir
	$(MAKE) SRCTYPE=SRCCLT $(ABI)/libTLM.a

lib_s:
	$(MAKE) dir
	$(MAKE) SRCTYPE=SRCSRVLIB $(ABI)/libTLM_s.a

manager: 
	$(MAKE) dir
	$(MAKE) SRCTYPE=SRCMGR $(ABI)/tlmmanager$(FEXT)

monitor:
	$(MAKE) dir
	$(MAKE) SRCTYPE=SRCMONITOR $(ABI)/tlmmonitor$(FEXT)

test:
	$(MAKE) dir
	$(MAKE) $(ABI)/testapp$(FEXT)

install: manager monitor
	cp $(ABI)/tlmmonitor$(FEXT) $(ABI)/tlmmanager$(FEXT) ../bin

$(ABI)/libTLM.a: $(OBJS)
	$(MAKE) dir
	@echo Use TLM-library for client applications using misc library.
	@echo Use TLM_m library otherwise.
	$(AR) ruv $(ABI)/libTLM.a $(OBJS) 

$(ABI)/libTLM_s.a: $(OBJS)
	$(AR) ruv $(ABI)/libTLM_s.a $(OBJS) 

$(ABI)/tlmmanager$(FEXT): $(OBJS)
	$(MAKE) dir
	$(LINK) -o $(ABI)/tlmmanager$(FEXT) $(OBJS) $(LIBS) $(XTRLIBS) $(LIBXML) $(LIBPTHREAD)
	if [ -f $(ABI)/tlmmanager.exe.manifest ] ; then \
		(cd $(ABI) ; mt.exe -manifest tlmmanager.exe.manifest -outputresource:tlmmanager.exe\;1); fi
	$(CP) $(ABI)/tlmmanager$(FEXT) $(BINDIR)/tlmmanager$(FEXT)

$(ABI)/tlmmonitor$(FEXT): $(OBJS)
	$(MAKE) dir
	$(LINK) -o $(ABI)/tlmmonitor$(FEXT) $(OBJS) $(LIBS) $(XTRLIBS) $(LIBXML) $(LIBPTHREAD)
	if [ -f $(ABI)/tlmmonitor.exe.manifest ] ; then \
		(cd $(ABI) ; mt.exe -manifest tlmmonitor.exe.manifest -outputresource:tlmmonitor.exe\;1); fi
	$(CP) $(ABI)/tlmmonitor$(FEXT) $(BINDIR)/tlmmonitor$(FEXT)

$(ABI)/testapp$(FEXT): $(ABI)/TLMTestApp.o
	$(LINK) $(ABI)/TLMTestApp.o -o $(ABI)/testapp$(FEXT) -L$(ABI) -lTLM $(LIBS) $(XTRLIBS) $(LIBPTHREAD)

$(ABI)/%.o: %.cc
	$(CC) $(DEFINES) $(CCFLAGS) $(OPTFLAGS4) $(INCLUDES) $(INCLXML) -c $< -o $@

.PHONY: clean dir depend lib manager test

clean:
	rm -rf $(ABI)
	rm -rf $(BINDIR)/tlmmanager$(FEXT) $(BINDIR)/tlmmonitor$(FEXT)

# Change 080701: $ABI is used for *.o files and .tail files

dir:
	@ ( if [ ! -d ./$(ABI) ] ; then mkdir -p ./$(ABI) ; fi)

$(ABI)/makedepend.tail: $(ABI)
	touch $@
$(ABI):
	mkdir -p $@
	mkdir -p $@/Interfaces
	mkdir -p $@/CompositeModels
	mkdir -p $@/Plugin
	mkdir -p $@/Communication
	mkdir -p $@/Parameters
	mkdir -p $@/Logging
depend: 
	@echo > $(ABI)/makedepend.tail
	makedepend -Y -p"\$$(ABI)/" -f $(ABI)/makedepend.tail \
			$(INCLUDES) $(SRCCLT) $(SRCMGR) $(FORDEPEND) 2> /dev/null

# if this file doesn't exist, a touch may be enough.
-include $(ABI)/makedepend.tail
