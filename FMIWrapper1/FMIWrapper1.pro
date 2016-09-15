TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

LIBS += -ldl

DEFINES += HAVE_MEMMOVE
DEFINES += HAVE_STDARG_H

#FMILibrary include paths
INCLUDEPATH += FMILibrary-2.0.1/install/include
INCLUDEPATH += FMILibrary-2.0.1/install/include/JM
INCLUDEPATH += FMILibrary-2.0.1/build-fmil
INCLUDEPATH += FMILibrary-2.0.1/src/CAPI/include
INCLUDEPATH += FMILibrary-2.0.1/src/CAPI/src
INCLUDEPATH += FMILibrary-2.0.1/src/XML/include
INCLUDEPATH += FMILibrary-2.0.1/src/ZIP/include
INCLUDEPATH += FMILibrary-2.0.1/ThirdParty/Minizip/minizip

#TLMPLugin include paths
INCLUDEPATH += ../common
INCLUDEPATH += ../extralibs/misc/include

SOURCES += main.cpp \
    FMILibrary-2.0.1/src/Import/src/FMI2/fmi2_import_capi.c \
    FMILibrary-2.0.1/src/Import/src/FMI2/fmi2_import_convenience.c \
#    FMILibrary-2.0.1/src/Import/src/FMI2/fmi2_import_type.c \
#    FMILibrary-2.0.1/src/Import/src/FMI2/fmi2_import_unit.c \
    FMILibrary-2.0.1/src/Import/src/FMI2/fmi2_import_variable_list.c \
#    FMILibrary-2.0.1/src/Import/src/FMI2/fmi2_import_variable.c \
    FMILibrary-2.0.1/src/Import/src/FMI2/fmi2_import.c \
    FMILibrary-2.0.1/src/XML/src/FMI2/fmi2_xml_model_description.c \
    FMILibrary-2.0.1/src/XML/src/FMI2/fmi2_xml_model_structure.c \
    FMILibrary-2.0.1/src/XML/src/FMI2/fmi2_xml_parser.c \
    FMILibrary-2.0.1/src/XML/src/FMI2/fmi2_xml_type.c \
    FMILibrary-2.0.1/src/XML/src/FMI2/fmi2_xml_unit.c \
    FMILibrary-2.0.1/src/XML/src/FMI2/fmi2_xml_variable.c \
    FMILibrary-2.0.1/src/XML/src/FMI2/fmi2_xml_vendor_annotations.c \
    FMILibrary-2.0.1/src/Util/src/JM/jm_callbacks.c \
    FMILibrary-2.0.1/src/Util/src/JM/jm_named_ptr.c \
    FMILibrary-2.0.1/src/Util/src/JM/jm_portability.c \
    FMILibrary-2.0.1/src/Util/src/JM/jm_templates_inst.c \
    FMILibrary-2.0.1/src/CAPI/src/FMI2/fmi2_capi_cs.c \
    FMILibrary-2.0.1/src/CAPI/src/FMI2/fmi2_capi_me.c \
    FMILibrary-2.0.1/src/CAPI/src/FMI2/fmi2_capi.c \
    FMILibrary-2.0.1/src/Import/src/FMI/fmi_import_context.c \
    FMILibrary-2.0.1/src/Import/src/FMI/fmi_import_util.c \
    FMILibrary-2.0.1/src/ZIP/src/fmi_zip_unzip.c \
#    FMILibrary-2.0.1/src/ZIP/src/fmi_zip_zip.c \
    #FMILibrary-2.0.1/build-fmil/CMakeFiles/3.2.2/CompilerIdCXX/CMakeCXXCompilerId.cpp \
#    FMILibrary-2.0.1/build-fmil/CMakeFiles/feature_tests.cxx \
#    FMILibrary-2.0.1/build-fmil/ExpatEx/CMakeFiles/3.2.2/CompilerIdCXX/CMakeCXXCompilerId.cpp \
 #   FMILibrary-2.0.1/build-fmil/ExpatEx/CMakeFiles/feature_tests.cxx \
    #FMILibrary-2.0.1/Test/FMI1/fmi2_import_xml_test.cc \
    #FMILibrary-2.0.1/Test/FMI1/fmi_import_xml_test.cc \
    #FMILibrary-2.0.1/Test/FMI2/fmi2_import_xml_test.cc \
    #FMILibrary-2.0.1/build-fmil/CMakeFiles/3.2.2/CompilerIdC/CMakeCCompilerId.c \
    #FMILibrary-2.0.1/build-fmil/CMakeFiles/CheckTypeSize/HAVE_INTMAX_T.c \
    #FMILibrary-2.0.1/build-fmil/CMakeFiles/CheckTypeSize/HAVE_LONG_DOUBLE.c \
    #FMILibrary-2.0.1/build-fmil/CMakeFiles/CheckTypeSize/HAVE_LONG_LONG_INT.c \
    #FMILibrary-2.0.1/build-fmil/CMakeFiles/CheckTypeSize/HAVE_PTRDIFF_T.c \
    #FMILibrary-2.0.1/build-fmil/CMakeFiles/CheckTypeSize/HAVE_UINTMAX_T.c \
    #FMILibrary-2.0.1/build-fmil/CMakeFiles/CheckTypeSize/HAVE_UINTPTR_T.c \
    #FMILibrary-2.0.1/build-fmil/CMakeFiles/CheckTypeSize/HAVE_UNSIGNED_LONG_LONG_INT.c \
    #FMILibrary-2.0.1/build-fmil/CMakeFiles/CheckTypeSize/LCONV.c \
    #FMILibrary-2.0.1/build-fmil/CMakeFiles/feature_tests.c \
    #FMILibrary-2.0.1/build-fmil/ExpatEx/CMakeFiles/3.2.2/CompilerIdC/CMakeCCompilerId.c \
    #FMILibrary-2.0.1/build-fmil/ExpatEx/CMakeFiles/CheckTypeSize/CMAKE_SIZEOF_UNSIGNED_SHORT.c \
    #FMILibrary-2.0.1/build-fmil/ExpatEx/CMakeFiles/feature_tests.c \
    #FMILibrary-2.0.1/build-fmil/fmilib_dummy.c \
    #FMILibrary-2.0.1/src/CAPI/src/FMI1/fmi1_capi.c \
    #FMILibrary-2.0.1/src/CAPI/src/FMI1/fmi1_capi_cs.c \
    #FMILibrary-2.0.1/src/CAPI/src/FMI1/fmi1_capi_me.c \
#    FMILibrary-2.0.1/src/Import/src/FMI1/fmi1_import.c \
#    FMILibrary-2.0.1/src/Import/src/FMI1/fmi1_import_capabilities.c \
#    FMILibrary-2.0.1/src/Import/src/FMI1/fmi1_import_capi.c \
#    FMILibrary-2.0.1/src/Import/src/FMI1/fmi1_import_convenience.c \
#    FMILibrary-2.0.1/src/Import/src/FMI1/fmi1_import_cosim.c \
#    FMILibrary-2.0.1/src/Import/src/FMI1/fmi1_import_type.c \
#    FMILibrary-2.0.1/src/Import/src/FMI1/fmi1_import_unit.c \
#    FMILibrary-2.0.1/src/Import/src/FMI1/fmi1_import_variable.c \
#    FMILibrary-2.0.1/src/Import/src/FMI1/fmi1_import_variable_list.c \
#    FMILibrary-2.0.1/src/Import/src/FMI1/fmi1_import_vendor_annotations.c \
   FMILibrary-2.0.1/src/Util/src/FMI/fmi_util.c \
    FMILibrary-2.0.1/src/Util/src/FMI/fmi_version.c \
#    FMILibrary-2.0.1/src/Util/src/FMI1/fmi1_enums.c \
    FMILibrary-2.0.1/src/Util/src/FMI2/fmi2_enums.c \
    FMILibrary-2.0.1/src/XML/src/FMI/fmi_xml_context.c \
#    FMILibrary-2.0.1/src/XML/src/FMI1/fmi1_xml_capabilities.c \
#    FMILibrary-2.0.1/src/XML/src/FMI1/fmi1_xml_cosim.c \
#    FMILibrary-2.0.1/src/XML/src/FMI1/fmi1_xml_model_description.c \
#    FMILibrary-2.0.1/src/XML/src/FMI1/fmi1_xml_parser.c \
#    FMILibrary-2.0.1/src/XML/src/FMI1/fmi1_xml_query.c \
#    FMILibrary-2.0.1/src/XML/src/FMI1/fmi1_xml_type.c \
#    FMILibrary-2.0.1/src/XML/src/FMI1/fmi1_xml_unit.c \
#    FMILibrary-2.0.1/src/XML/src/FMI1/fmi1_xml_variable.c \
#    FMILibrary-2.0.1/src/XML/src/FMI1/fmi1_xml_vendor_annotations.c \
  #    FMILibrary-2.0.1/src/XML/src/FMI2/fmi2_xml_query.c \
#    FMILibrary-2.0.1/Test/FMI1/fmu_dummy/fmu1_model.c \
#    FMILibrary-2.0.1/Test/FMI1/fmu_dummy/fmu1_model_cs.c \
#    FMILibrary-2.0.1/Test/FMI1/fmu_dummy/fmu1_model_me.c \
#    FMILibrary-2.0.1/Test/FMI1/compress_test_fmu_zip.c \
#    FMILibrary-2.0.1/Test/FMI1/fmi1_capi_cs_test.c \
#    FMILibrary-2.0.1/Test/FMI1/fmi1_capi_me_test.c \
#    FMILibrary-2.0.1/Test/FMI1/fmi1_import_test.c \
#    FMILibrary-2.0.1/Test/FMI1/fmi1_logger_test.c \
#    FMILibrary-2.0.1/Test/FMI1/fmi_import_cs_test.c \
#    FMILibrary-2.0.1/Test/FMI1/fmi_import_me_test.c \
#    FMILibrary-2.0.1/Test/FMI1/fmi_total_test.c \
#    FMILibrary-2.0.1/Test/FMI1/fmi_zip_unzip_test.c \
#    FMILibrary-2.0.1/Test/FMI1/fmi_zip_zip_test.c \
#    FMILibrary-2.0.1/Test/FMI1/jm_vector_test.c \
#    FMILibrary-2.0.1/Test/FMI2/fmu_dummy/fmu2_model.c \
#    FMILibrary-2.0.1/Test/FMI2/fmu_dummy/fmu2_model_cs.c \
#    FMILibrary-2.0.1/Test/FMI2/fmu_dummy/fmu2_model_me.c \
#    FMILibrary-2.0.1/Test/FMI2/fmi2_import_cs_test.c \
#    FMILibrary-2.0.1/Test/FMI2/fmi2_import_me_test.c \
#    FMILibrary-2.0.1/Test/FMI2/fmi2_import_test.c \
#    FMILibrary-2.0.1/Test/compress_test_fmu_zip.c \
#    FMILibrary-2.0.1/Test/fmi_import_test.c \
#    FMILibrary-2.0.1/Test/fmi_zip_unzip_test.c \
#    FMILibrary-2.0.1/Test/fmi_zip_zip_test.c \
#    FMILibrary-2.0.1/Test/jm_vector_test.c \
    FMILibrary-2.0.1/ThirdParty/c99_snprintf/c99-snprintf_1.1/snprintf.c \
#
  #    FMILibrary-2.0.1/ThirdParty/Expat/expat-2.1.0/amiga/expat_68k.c \
#    FMILibrary-2.0.1/ThirdParty/Expat/expat-2.1.0/amiga/expat_68k_handler_stubs.c \
#    FMILibrary-2.0.1/ThirdParty/Expat/expat-2.1.0/amiga/expat_lib.c \
#    FMILibrary-2.0.1/ThirdParty/Expat/expat-2.1.0/amiga/expat_vectors.c \
#    FMILibrary-2.0.1/ThirdParty/Expat/expat-2.1.0/amiga/launch.c \
    FMILibrary-2.0.1/ThirdParty/Expat/expat-2.1.0/lib/xmlparse.c \
    FMILibrary-2.0.1/ThirdParty/Expat/expat-2.1.0/lib/xmlrole.c \
   FMILibrary-2.0.1/ThirdParty/Expat/expat-2.1.0/lib/xmltok.c \
    FMILibrary-2.0.1/ThirdParty/Expat/expat-2.1.0/lib/xmltok_impl.c \
    FMILibrary-2.0.1/ThirdParty/Expat/expat-2.1.0/lib/xmltok_ns.c \
#    FMILibrary-2.0.1/ThirdParty/Expat/expat-2.1.0/xmlwf/codepage.c \
#    FMILibrary-2.0.1/ThirdParty/Expat/expat-2.1.0/xmlwf/ct.c \
#    FMILibrary-2.0.1/ThirdParty/Expat/expat-2.1.0/xmlwf/readfilemap.c \
#    FMILibrary-2.0.1/ThirdParty/Expat/expat-2.1.0/xmlwf/unixfilemap.c \
#    FMILibrary-2.0.1/ThirdParty/Expat/expat-2.1.0/xmlwf/win32filemap.c \
#    FMILibrary-2.0.1/ThirdParty/Expat/expat-2.1.0/xmlwf/xmlfile.c \
#    FMILibrary-2.0.1/ThirdParty/Expat/expat-2.1.0/xmlwf/xmlmime.c \
#    FMILibrary-2.0.1/ThirdParty/Expat/expat-2.1.0/xmlwf/xmlwf.c \
#
       FMILibrary-2.0.1/ThirdParty/Minizip/minizip/ioapi.c \
#    FMILibrary-2.0.1/ThirdParty/Minizip/minizip/iowin32.c \
       FMILibrary-2.0.1/ThirdParty/Minizip/minizip/miniunz.c \
    FMILibrary-2.0.1/ThirdParty/Minizip/minizip/minizip.c \
    FMILibrary-2.0.1/ThirdParty/Minizip/minizip/mztools.c \
       FMILibrary-2.0.1/ThirdParty/Minizip/minizip/unzip.c \
    FMILibrary-2.0.1/ThirdParty/Minizip/minizip/zip.c \
    FMILibrary-2.0.1/ThirdParty/Zlib/zlib-1.2.6/adler32.c \
    FMILibrary-2.0.1/ThirdParty/Zlib/zlib-1.2.6/compress.c \
    FMILibrary-2.0.1/ThirdParty/Zlib/zlib-1.2.6/crc32.c \
    FMILibrary-2.0.1/ThirdParty/Zlib/zlib-1.2.6/deflate.c \
    FMILibrary-2.0.1/ThirdParty/Zlib/zlib-1.2.6/gzclose.c \
    FMILibrary-2.0.1/ThirdParty/Zlib/zlib-1.2.6/gzlib.c \
    FMILibrary-2.0.1/ThirdParty/Zlib/zlib-1.2.6/gzread.c \
    FMILibrary-2.0.1/ThirdParty/Zlib/zlib-1.2.6/gzwrite.c \
    FMILibrary-2.0.1/ThirdParty/Zlib/zlib-1.2.6/infback.c \
    FMILibrary-2.0.1/ThirdParty/Zlib/zlib-1.2.6/inffast.c \
       FMILibrary-2.0.1/ThirdParty/Zlib/zlib-1.2.6/inflate.c \
    FMILibrary-2.0.1/ThirdParty/Zlib/zlib-1.2.6/inftrees.c \
    FMILibrary-2.0.1/ThirdParty/Zlib/zlib-1.2.6/trees.c \
    FMILibrary-2.0.1/ThirdParty/Zlib/zlib-1.2.6/uncompr.c \
    FMILibrary-2.0.1/ThirdParty/Zlib/zlib-1.2.6/zutil.c \
    ../common/PluginImplementer.cc \
    ../common/TLMClientComm.cc \
    ../common/TLMCommUtil.cc \
    ../common/TLMErrorLog.cc \
    ../common/TLMInterface.cc \
    ../common/TLMPlugin.cc \
    ../extralibs/misc/src/strConv.cc \
    ../extralibs/misc/src/coordTrans.cc \
    ../extralibs/misc/src/double3Vec.cc \
    ../extralibs/misc/src/double33Mat.cc

HEADERS += \
#    FMILibrary-2.0.1/src/Import/src/FMI2/fmi2_import_impl.h \
#    FMILibrary-2.0.1/src/Import/src/FMI2/fmi2_import_variable_list_impl.h \
#    FMILibrary-2.0.1/src/XML/src/FMI2/fmi2_xml_model_description_impl.h \
#    FMILibrary-2.0.1/src/XML/src/FMI2/fmi2_xml_model_structure_impl.h \
#    FMILibrary-2.0.1/src/XML/src/FMI2/fmi2_xml_parser.h \
#    FMILibrary-2.0.1/src/XML/src/FMI2/fmi2_xml_query.h \
#    FMILibrary-2.0.1/src/XML/src/FMI2/fmi2_xml_type_impl.h \
#    FMILibrary-2.0.1/src/XML/src/FMI2/fmi2_xml_unit_impl.h \
#    FMILibrary-2.0.1/src/XML/src/FMI2/fmi2_xml_variable_impl.h \
#    FMILibrary-2.0.1/src/CAPI/src/FMI2/fmi2_capi_impl.h \
#    FMILibrary-2.0.1/src/Import/src/FMI/fmi_import_context_impl.h \
#    FMILibrary-2.0.1/build-fmil/doc/fmilib.h \
#    FMILibrary-2.0.1/build-fmil/doc/fmilib_config.h \
#    FMILibrary-2.0.1/build-fmil/doc/fmilib_mainpage.h \
#    FMILibrary-2.0.1/build-fmil/ExpatEx/install/include/expat.h \
#    FMILibrary-2.0.1/build-fmil/ExpatEx/install/include/expat_external.h \
#    FMILibrary-2.0.1/build-fmil/ExpatEx/expat_config.h \
#    FMILibrary-2.0.1/build-fmil/zlib/zconf.h \
#    FMILibrary-2.0.1/build-fmil/config_test.h \
#    FMILibrary-2.0.1/build-fmil/fmilib.h \
#    FMILibrary-2.0.1/build-fmil/fmilib_config.h \
#    FMILibrary-2.0.1/Config.cmake/fmilib.h \
#    FMILibrary-2.0.1/install/include/FMI/fmi_import_context.h \
#    FMILibrary-2.0.1/install/include/FMI/fmi_import_util.h \
#    FMILibrary-2.0.1/install/include/FMI/fmi_util.h \
#    FMILibrary-2.0.1/install/include/FMI/fmi_version.h \
#    FMILibrary-2.0.1/install/include/FMI1/fmi1_enums.h \
#    FMILibrary-2.0.1/install/include/FMI1/fmi1_functions.h \
#    FMILibrary-2.0.1/install/include/FMI1/fmi1_import.h \
#    FMILibrary-2.0.1/install/include/FMI1/fmi1_import_capabilities.h \
#    FMILibrary-2.0.1/install/include/FMI1/fmi1_import_capi.h \
#    FMILibrary-2.0.1/install/include/FMI1/fmi1_import_convenience.h \
#    FMILibrary-2.0.1/install/include/FMI1/fmi1_import_cosim.h \
#    FMILibrary-2.0.1/install/include/FMI1/fmi1_import_type.h \
#    FMILibrary-2.0.1/install/include/FMI1/fmi1_import_unit.h \
#    FMILibrary-2.0.1/install/include/FMI1/fmi1_import_variable.h \
#    FMILibrary-2.0.1/install/include/FMI1/fmi1_import_variable_list.h \
#    FMILibrary-2.0.1/install/include/FMI1/fmi1_import_vendor_annotations.h \
#    FMILibrary-2.0.1/install/include/FMI1/fmi1_types.h \
#    FMILibrary-2.0.1/install/include/FMI1/fmiFunctions.h \
#    FMILibrary-2.0.1/install/include/FMI1/fmiModelFunctions.h \
#    FMILibrary-2.0.1/install/include/FMI1/fmiModelTypes.h \
#    FMILibrary-2.0.1/install/include/FMI1/fmiPlatformTypes.h \
#    FMILibrary-2.0.1/install/include/FMI2/fmi2_enums.h \
#    FMILibrary-2.0.1/install/include/FMI2/fmi2_functions.h \
#    FMILibrary-2.0.1/install/include/FMI2/fmi2_import.h \
#    FMILibrary-2.0.1/install/include/FMI2/fmi2_import_capi.h \
#    FMILibrary-2.0.1/install/include/FMI2/fmi2_import_convenience.h \
#    FMILibrary-2.0.1/install/include/FMI2/fmi2_import_type.h \
#    FMILibrary-2.0.1/install/include/FMI2/fmi2_import_unit.h \
#    FMILibrary-2.0.1/install/include/FMI2/fmi2_import_variable.h \
#    FMILibrary-2.0.1/install/include/FMI2/fmi2_import_variable_list.h \
#    FMILibrary-2.0.1/install/include/FMI2/fmi2_types.h \
#    FMILibrary-2.0.1/install/include/FMI2/fmi2_xml_callbacks.h \
#    FMILibrary-2.0.1/install/include/FMI2/fmi2Functions.h \
#    FMILibrary-2.0.1/install/include/FMI2/fmi2FunctionTypes.h \
#    FMILibrary-2.0.1/install/include/FMI2/fmi2TypesPlatform.h \
#    FMILibrary-2.0.1/install/include/JM/jm_callbacks.h \
#    FMILibrary-2.0.1/install/include/JM/jm_named_ptr.h \
#    FMILibrary-2.0.1/install/include/JM/jm_portability.h \
#    FMILibrary-2.0.1/install/include/JM/jm_stack.h \
#    FMILibrary-2.0.1/install/include/JM/jm_string_set.h \
#    FMILibrary-2.0.1/install/include/JM/jm_types.h \
#    FMILibrary-2.0.1/install/include/JM/jm_vector.h \
#    FMILibrary-2.0.1/install/include/JM/jm_vector_template.h \
#    FMILibrary-2.0.1/install/include/fmilib.h \
#    FMILibrary-2.0.1/install/include/fmilib_config.h \
#    FMILibrary-2.0.1/src/CAPI/include/FMI1/fmi1_capi.h \
#    FMILibrary-2.0.1/src/CAPI/include/FMI2/fmi2_capi.h \
#    FMILibrary-2.0.1/src/CAPI/src/FMI1/fmi1_capi_impl.h \
#    FMILibrary-2.0.1/src/Import/include/FMI/fmi_import_context.h \
#    FMILibrary-2.0.1/src/Import/include/FMI/fmi_import_util.h \
#    FMILibrary-2.0.1/src/Import/include/FMI1/fmi1_import.h \
#    FMILibrary-2.0.1/src/Import/include/FMI1/fmi1_import_capabilities.h \
#    FMILibrary-2.0.1/src/Import/include/FMI1/fmi1_import_capi.h \
#    FMILibrary-2.0.1/src/Import/include/FMI1/fmi1_import_convenience.h \
#    FMILibrary-2.0.1/src/Import/include/FMI1/fmi1_import_cosim.h \
#    FMILibrary-2.0.1/src/Import/include/FMI1/fmi1_import_type.h \
#    FMILibrary-2.0.1/src/Import/include/FMI1/fmi1_import_unit.h \
#    FMILibrary-2.0.1/src/Import/include/FMI1/fmi1_import_variable.h \
#    FMILibrary-2.0.1/src/Import/include/FMI1/fmi1_import_variable_list.h \
#    FMILibrary-2.0.1/src/Import/include/FMI1/fmi1_import_vendor_annotations.h \
#    FMILibrary-2.0.1/src/Import/include/FMI2/fmi2_import.h \
#    FMILibrary-2.0.1/src/Import/include/FMI2/fmi2_import_capi.h \
#    FMILibrary-2.0.1/src/Import/include/FMI2/fmi2_import_convenience.h \
#    FMILibrary-2.0.1/src/Import/include/FMI2/fmi2_import_type.h \
#    FMILibrary-2.0.1/src/Import/include/FMI2/fmi2_import_unit.h \
#    FMILibrary-2.0.1/src/Import/include/FMI2/fmi2_import_variable.h \
#    FMILibrary-2.0.1/src/Import/include/FMI2/fmi2_import_variable_list.h \
#    FMILibrary-2.0.1/src/Import/src/FMI1/fmi1_import_impl.h \
#    FMILibrary-2.0.1/src/Import/src/FMI1/fmi1_import_variable_list_impl.h \
#    FMILibrary-2.0.1/src/Util/include/FMI/fmi_util.h \
#    FMILibrary-2.0.1/src/Util/include/FMI/fmi_version.h \
#    FMILibrary-2.0.1/src/Util/include/FMI1/fmi1_enums.h \
#    FMILibrary-2.0.1/src/Util/include/FMI1/fmi1_functions.h \
#    FMILibrary-2.0.1/src/Util/include/FMI1/fmi1_types.h \
#    FMILibrary-2.0.1/src/Util/include/FMI2/fmi2_enums.h \
#    FMILibrary-2.0.1/src/Util/include/FMI2/fmi2_functions.h \
#    FMILibrary-2.0.1/src/Util/include/FMI2/fmi2_types.h \
#    FMILibrary-2.0.1/src/Util/include/FMI2/fmi2_xml_callbacks.h \
#    FMILibrary-2.0.1/src/Util/include/JM/jm_callbacks.h \
#    FMILibrary-2.0.1/src/Util/include/JM/jm_named_ptr.h \
#    FMILibrary-2.0.1/src/Util/include/JM/jm_portability.h \
#    FMILibrary-2.0.1/src/Util/include/JM/jm_stack.h \
#    FMILibrary-2.0.1/src/Util/include/JM/jm_string_set.h \
#    FMILibrary-2.0.1/src/Util/include/JM/jm_types.h \
#    FMILibrary-2.0.1/src/Util/include/JM/jm_vector.h \
#    FMILibrary-2.0.1/src/Util/include/JM/jm_vector_template.h \
#    FMILibrary-2.0.1/src/XML/include/FMI/fmi_xml_context.h \
#    FMILibrary-2.0.1/src/XML/include/FMI1/fmi1_xml_capabilities.h \
#    FMILibrary-2.0.1/src/XML/include/FMI1/fmi1_xml_cosim.h \
#    FMILibrary-2.0.1/src/XML/include/FMI1/fmi1_xml_model_description.h \
#    FMILibrary-2.0.1/src/XML/include/FMI1/fmi1_xml_type.h \
#    FMILibrary-2.0.1/src/XML/include/FMI1/fmi1_xml_unit.h \
#    FMILibrary-2.0.1/src/XML/include/FMI1/fmi1_xml_variable.h \
#    FMILibrary-2.0.1/src/XML/include/FMI1/fmi1_xml_vendor_annotations.h \
#    FMILibrary-2.0.1/src/XML/include/FMI2/fmi2_xml_capabilities.h \
#    FMILibrary-2.0.1/src/XML/include/FMI2/fmi2_xml_cosim.h \
#    FMILibrary-2.0.1/src/XML/include/FMI2/fmi2_xml_model_description.h \
#    FMILibrary-2.0.1/src/XML/include/FMI2/fmi2_xml_model_structure.h \
#    FMILibrary-2.0.1/src/XML/include/FMI2/fmi2_xml_type.h \
#    FMILibrary-2.0.1/src/XML/include/FMI2/fmi2_xml_unit.h \
#    FMILibrary-2.0.1/src/XML/include/FMI2/fmi2_xml_variable.h \
#    FMILibrary-2.0.1/src/XML/src/FMI/fmi_xml_context_impl.h \
#    FMILibrary-2.0.1/src/XML/src/FMI1/fmi1_xml_capabilities_impl.h \
#    FMILibrary-2.0.1/src/XML/src/FMI1/fmi1_xml_model_description_impl.h \
#    FMILibrary-2.0.1/src/XML/src/FMI1/fmi1_xml_parser.h \
#    FMILibrary-2.0.1/src/XML/src/FMI1/fmi1_xml_query.h \
#    FMILibrary-2.0.1/src/XML/src/FMI1/fmi1_xml_type_impl.h \
#    FMILibrary-2.0.1/src/XML/src/FMI1/fmi1_xml_unit_impl.h \
#    FMILibrary-2.0.1/src/XML/src/FMI1/fmi1_xml_variable_impl.h \
#    FMILibrary-2.0.1/src/XML/src/FMI1/fmi1_xml_vendor_annotations_impl.h \
#    FMILibrary-2.0.1/src/ZIP/include/FMI/fmi_zip_unzip.h \
#    FMILibrary-2.0.1/src/ZIP/include/FMI/fmi_zip_zip.h \
#    FMILibrary-2.0.1/Test/FMI1/fmu_dummy/fmu1_model.h \
#    FMILibrary-2.0.1/Test/FMI1/fmu_dummy/fmu1_model_defines.h \
#    FMILibrary-2.0.1/Test/FMI2/fmu_dummy/fmu2_model.h \
#    FMILibrary-2.0.1/Test/FMI2/fmu_dummy/fmu2_model_defines.h \
#    FMILibrary-2.0.1/ThirdParty/c99_snprintf/c99-snprintf_1.1/system.h \
#    FMILibrary-2.0.1/ThirdParty/Expat/expat-2.1.0/amiga/include/inline4/expat.h \
#    FMILibrary-2.0.1/ThirdParty/Expat/expat-2.1.0/amiga/include/interfaces/expat.h \
#    FMILibrary-2.0.1/ThirdParty/Expat/expat-2.1.0/amiga/include/libraries/expat.h \
#    FMILibrary-2.0.1/ThirdParty/Expat/expat-2.1.0/amiga/include/proto/expat.h \
#    FMILibrary-2.0.1/ThirdParty/Expat/expat-2.1.0/amiga/expat_68k.h \
#    FMILibrary-2.0.1/ThirdParty/Expat/expat-2.1.0/amiga/expat_base.h \
#    FMILibrary-2.0.1/ThirdParty/Expat/expat-2.1.0/lib/amigaconfig.h \
#    FMILibrary-2.0.1/ThirdParty/Expat/expat-2.1.0/lib/ascii.h \
#    FMILibrary-2.0.1/ThirdParty/Expat/expat-2.1.0/lib/asciitab.h \
#    FMILibrary-2.0.1/ThirdParty/Expat/expat-2.1.0/lib/expat.h \
#    FMILibrary-2.0.1/ThirdParty/Expat/expat-2.1.0/lib/expat_external.h \
#    FMILibrary-2.0.1/ThirdParty/Expat/expat-2.1.0/lib/iasciitab.h \
#    FMILibrary-2.0.1/ThirdParty/Expat/expat-2.1.0/lib/internal.h \
#    FMILibrary-2.0.1/ThirdParty/Expat/expat-2.1.0/lib/latin1tab.h \
#    FMILibrary-2.0.1/ThirdParty/Expat/expat-2.1.0/lib/macconfig.h \
#    FMILibrary-2.0.1/ThirdParty/Expat/expat-2.1.0/lib/nametab.h \
#    FMILibrary-2.0.1/ThirdParty/Expat/expat-2.1.0/lib/utf8tab.h \
#    FMILibrary-2.0.1/ThirdParty/Expat/expat-2.1.0/lib/winconfig.h \
#    FMILibrary-2.0.1/ThirdParty/Expat/expat-2.1.0/lib/xmlrole.h \
#    FMILibrary-2.0.1/ThirdParty/Expat/expat-2.1.0/lib/xmltok.h \
#    FMILibrary-2.0.1/ThirdParty/Expat/expat-2.1.0/lib/xmltok_impl.h \
#    FMILibrary-2.0.1/ThirdParty/Expat/expat-2.1.0/tests/chardata.h \
#    FMILibrary-2.0.1/ThirdParty/Expat/expat-2.1.0/tests/minicheck.h \
#    FMILibrary-2.0.1/ThirdParty/Expat/expat-2.1.0/vms/expat_config.h \
#    FMILibrary-2.0.1/ThirdParty/Expat/expat-2.1.0/xmlwf/codepage.h \
#    FMILibrary-2.0.1/ThirdParty/Expat/expat-2.1.0/xmlwf/filemap.h \
#    FMILibrary-2.0.1/ThirdParty/Expat/expat-2.1.0/xmlwf/xmlfile.h \
#    FMILibrary-2.0.1/ThirdParty/Expat/expat-2.1.0/xmlwf/xmlmime.h \
#    FMILibrary-2.0.1/ThirdParty/Expat/expat-2.1.0/xmlwf/xmltchar.h \
#    FMILibrary-2.0.1/ThirdParty/Expat/expat-2.1.0/xmlwf/xmlurl.h \
#    FMILibrary-2.0.1/ThirdParty/FMI/default/FMI1/fmiFunctions.h \
#    FMILibrary-2.0.1/ThirdParty/FMI/default/FMI1/fmiModelFunctions.h \
#    FMILibrary-2.0.1/ThirdParty/FMI/default/FMI1/fmiModelTypes.h \
#    FMILibrary-2.0.1/ThirdParty/FMI/default/FMI1/fmiPlatformTypes.h \
#    FMILibrary-2.0.1/ThirdParty/FMI/default/FMI2/fmi2Functions.h \
#    FMILibrary-2.0.1/ThirdParty/FMI/default/FMI2/fmi2FunctionTypes.h \
#    FMILibrary-2.0.1/ThirdParty/FMI/default/FMI2/fmi2TypesPlatform.h \
#    FMILibrary-2.0.1/ThirdParty/FMI/standard32/FMI1/fmiFunctions.h \
#    FMILibrary-2.0.1/ThirdParty/FMI/standard32/FMI1/fmiModelFunctions.h \
#    FMILibrary-2.0.1/ThirdParty/FMI/standard32/FMI1/fmiModelTypes.h \
#    FMILibrary-2.0.1/ThirdParty/FMI/standard32/FMI1/fmiPlatformTypes.h \
#    FMILibrary-2.0.1/ThirdParty/Minizip/minizip/crypt.h \
#    FMILibrary-2.0.1/ThirdParty/Minizip/minizip/ioapi.h \
#    FMILibrary-2.0.1/ThirdParty/Minizip/minizip/iowin32.h \
#    FMILibrary-2.0.1/ThirdParty/Minizip/minizip/miniunz.h \
#    FMILibrary-2.0.1/ThirdParty/Minizip/minizip/minizip.h \
#    FMILibrary-2.0.1/ThirdParty/Minizip/minizip/mztools.h \
#    FMILibrary-2.0.1/ThirdParty/Minizip/minizip/unzip.h \
#    FMILibrary-2.0.1/ThirdParty/Minizip/minizip/zip.h \
#    FMILibrary-2.0.1/ThirdParty/Zlib/zlib-1.2.6/examples/gzlog.h \
#    FMILibrary-2.0.1/ThirdParty/Zlib/zlib-1.2.6/crc32.h \
#    FMILibrary-2.0.1/ThirdParty/Zlib/zlib-1.2.6/deflate.h \
#    FMILibrary-2.0.1/ThirdParty/Zlib/zlib-1.2.6/gzguts.h \
#    FMILibrary-2.0.1/ThirdParty/Zlib/zlib-1.2.6/inffast.h \
#    FMILibrary-2.0.1/ThirdParty/Zlib/zlib-1.2.6/inffixed.h \
#    FMILibrary-2.0.1/ThirdParty/Zlib/zlib-1.2.6/inflate.h \
#    FMILibrary-2.0.1/ThirdParty/Zlib/zlib-1.2.6/inftrees.h \
#    FMILibrary-2.0.1/ThirdParty/Zlib/zlib-1.2.6/trees.h \
#    FMILibrary-2.0.1/ThirdParty/Zlib/zlib-1.2.6/zlib.h \
#    FMILibrary-2.0.1/ThirdParty/Zlib/zlib-1.2.6/zutil.h

