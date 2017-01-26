# Install script for directory: C:/Users/robbr48/Documents/Mercurial/TLMPlugin/FMIWrapper/cvode-2.9.0/src/cvode

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "C:/Users/robbr48/Documents/Mercurial/TLMPlugin/FMIWrapper/cvode-2.9.0/instdir")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  MESSAGE("
Install CVODE
")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "C:/Users/robbr48/Documents/Mercurial/TLMPlugin/FMIWrapper/cvode-2.9.0/builddir/src/cvode/libsundials_cvode.a")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY OPTIONAL FILES "C:/Users/robbr48/Documents/Mercurial/TLMPlugin/FMIWrapper/cvode-2.9.0/builddir/src/cvode/libsundials_cvode.dll.a")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE SHARED_LIBRARY FILES "C:/Users/robbr48/Documents/Mercurial/TLMPlugin/FMIWrapper/cvode-2.9.0/builddir/src/cvode/libsundials_cvode.dll")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libsundials_cvode.dll" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libsundials_cvode.dll")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "C:/mingw64/bin/strip.exe" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libsundials_cvode.dll")
    endif()
  endif()
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/cvode" TYPE FILE FILES
    "C:/Users/robbr48/Documents/Mercurial/TLMPlugin/FMIWrapper/cvode-2.9.0/include/cvode/cvode_band.h"
    "C:/Users/robbr48/Documents/Mercurial/TLMPlugin/FMIWrapper/cvode-2.9.0/include/cvode/cvode_bandpre.h"
    "C:/Users/robbr48/Documents/Mercurial/TLMPlugin/FMIWrapper/cvode-2.9.0/include/cvode/cvode_bbdpre.h"
    "C:/Users/robbr48/Documents/Mercurial/TLMPlugin/FMIWrapper/cvode-2.9.0/include/cvode/cvode_dense.h"
    "C:/Users/robbr48/Documents/Mercurial/TLMPlugin/FMIWrapper/cvode-2.9.0/include/cvode/cvode_diag.h"
    "C:/Users/robbr48/Documents/Mercurial/TLMPlugin/FMIWrapper/cvode-2.9.0/include/cvode/cvode_direct.h"
    "C:/Users/robbr48/Documents/Mercurial/TLMPlugin/FMIWrapper/cvode-2.9.0/include/cvode/cvode.h"
    "C:/Users/robbr48/Documents/Mercurial/TLMPlugin/FMIWrapper/cvode-2.9.0/include/cvode/cvode_sparse.h"
    "C:/Users/robbr48/Documents/Mercurial/TLMPlugin/FMIWrapper/cvode-2.9.0/include/cvode/cvode_spbcgs.h"
    "C:/Users/robbr48/Documents/Mercurial/TLMPlugin/FMIWrapper/cvode-2.9.0/include/cvode/cvode_spgmr.h"
    "C:/Users/robbr48/Documents/Mercurial/TLMPlugin/FMIWrapper/cvode-2.9.0/include/cvode/cvode_spils.h"
    "C:/Users/robbr48/Documents/Mercurial/TLMPlugin/FMIWrapper/cvode-2.9.0/include/cvode/cvode_sptfqmr.h"
    )
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/cvode" TYPE FILE FILES "C:/Users/robbr48/Documents/Mercurial/TLMPlugin/FMIWrapper/cvode-2.9.0/src/cvode/cvode_impl.h")
endif()

