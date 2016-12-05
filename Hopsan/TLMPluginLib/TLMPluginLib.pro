# QT -= core gui, means that we should not link the default qt libs into the component
# Template = lib, means that we want to build a library (.dll or .so)
QT -= core gui
TEMPLATE = lib

# TARGET is the name of the compiled lib, (.dll or .so will be added automatically)
# Change this to the name of YOUR lib
TARGET = TLMPluginLib

# Destination for the compiled dll. $${PWD}/ means the same directory as this .pro file, even if you use shadow build
DESTDIR = $${PWD}/

#TLMPLugin include paths
INCLUDEPATH += ../../common
INCLUDEPATH += ../../extralibs/misc/include

unix {
LIBS += -ldl
}
win32 {
LIBS += -lws2_32
}

# The location to search for the Hopsan include files, by specifying the path here, you dont need to do this everywhere in all of your component .hpp files
# See myLocalPathTemplate.prf, copy this file (do not change it, change the copy to suit your needs)
!exists(myLocalPaths.prf){
    error("myLocalPaths.prf does not exist. You need to COPY the template file. DO NOT commit myLocalPaths.prf")
}
include(myLocalPaths.prf)

# Special options for deug and release mode. This will link the correct HopsanCore .dll or .so
# In debug mode HopsanCore has the debug extension _d
include(hopsanDebugReleaseCompile.prf)

# Reduce compile output clutter, but show warnings
#CONFIG += silent warn_on

# The compiler should be pedantic to catch all errors (optional)
#QMAKE_CXXFLAGS += -pedantic

# -------------------------------------------------
# Project files
# -------------------------------------------------
SOURCES += \
    TLMPluginLib.cc \
    ../../common/PluginImplementer.cc \
    ../../common/TLMClientComm.cc \
    ../../common/TLMCommUtil.cc \
    ../../common/TLMErrorLog.cc \
    ../../common/TLMInterface.cc \
    ../../common/TLMPlugin.cc \
    ../../extralibs/misc/src/strConv.cc \
    ../../extralibs/misc/src/coordTrans.cc \
    ../../extralibs/misc/src/double3Vec.cc \
    ../../extralibs/misc/src/double33Mat.cc \
    ../../common/TLMInterfaceSignal.cc \
    ../../common/TLMInterfaceSignalInput.cc \
    ../../common/TLMInterfaceSignalOutput.cc \
    ../../common/TLMInterface1D.cc \
    ../../common/TLMInterface3D.cc

HEADERS += \
    TLMPluginInterface1D.hpp \
    TLMPluginInterfaceSignalInput.hpp \
    TLMPluginInterfaceSignalOutput.hpp \
    common.h \
    TLMPluginHandler.hpp

OTHER_FILES += \
    hopsanDebugReleaseCompile.prf \
    myLocalPaths.prf

DISTFILES += \
    TLMPluginInterface1D.xml \
    TLMPluginInterface1D.svg \
    TLMPluginInterfaceSignalInput.xml \
    TLMPluginInterfaceSignalOutput.xml \
    TLMPluginInterfaceSignalInput.svg \
    TLMPluginInterfaceSignalOutput.svg \
    TLMPluginLib.xml \
    TLMPluginHandler.xml \
    TLMPluginHandler.svg
