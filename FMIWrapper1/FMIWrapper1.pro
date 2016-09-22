TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

TARGET = ../../bin/FMIWrapper1

unix {
LIBS += -ldl
}
win32 {
LIBS += -lws2_32
}

LIBS += -L$${PWD}/FMILibrary-2.0.1/install/lib
LIBS += -llibfmilib_shared

#FMILibrary include path
INCLUDEPATH += FMILibrary-2.0.1/install/include

#TLMPLugin include paths
INCLUDEPATH += ../common
INCLUDEPATH += ../extralibs/misc/include

SOURCES += main.cpp \
    ../common/PluginImplementer.cc \
    ../common/TLMClientComm.cc \
    ../common/TLMCommUtil.cc \
    ../common/TLMErrorLog.cc \
    ../common/TLMInterface.cc \
    ../common/TLMPlugin.cc \
    ../extralibs/misc/src/strConv.cc \
    ../extralibs/misc/src/coordTrans.cc \
    ../extralibs/misc/src/double3Vec.cc \
    ../extralibs/misc/src/double33Mat.cc #\

HEADERS += \
    FMILibrary-2.0.1/install/include/JM/jm_portability.h \
    FMILibrary-2.0.1/install/include/FMI/fmi_import_context.h \
    FMILibrary-2.0.1/install/include/FMI1/fmi1_import.h \
    FMILibrary-2.0.1/install/include/FMI2/fmi2_import.h \
    FMILibrary-2.0.1/install/include/fmilib_config.h

