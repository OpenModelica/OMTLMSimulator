# Install script for directory: C:/Users/robbr48/Documents/Mercurial/TLMPlugin/FMIWrapper/cvode-2.9.0/src/sundials

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
Install shared components
")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/sundials" TYPE FILE FILES
    "C:/Users/robbr48/Documents/Mercurial/TLMPlugin/FMIWrapper/cvode-2.9.0/include/sundials/sundials_band.h"
    "C:/Users/robbr48/Documents/Mercurial/TLMPlugin/FMIWrapper/cvode-2.9.0/include/sundials/sundials_dense.h"
    "C:/Users/robbr48/Documents/Mercurial/TLMPlugin/FMIWrapper/cvode-2.9.0/include/sundials/sundials_direct.h"
    "C:/Users/robbr48/Documents/Mercurial/TLMPlugin/FMIWrapper/cvode-2.9.0/include/sundials/sundials_iterative.h"
    "C:/Users/robbr48/Documents/Mercurial/TLMPlugin/FMIWrapper/cvode-2.9.0/include/sundials/sundials_math.h"
    "C:/Users/robbr48/Documents/Mercurial/TLMPlugin/FMIWrapper/cvode-2.9.0/include/sundials/sundials_nvector.h"
    "C:/Users/robbr48/Documents/Mercurial/TLMPlugin/FMIWrapper/cvode-2.9.0/include/sundials/sundials_fnvector.h"
    "C:/Users/robbr48/Documents/Mercurial/TLMPlugin/FMIWrapper/cvode-2.9.0/include/sundials/sundials_pcg.h"
    "C:/Users/robbr48/Documents/Mercurial/TLMPlugin/FMIWrapper/cvode-2.9.0/include/sundials/sundials_sparse.h"
    "C:/Users/robbr48/Documents/Mercurial/TLMPlugin/FMIWrapper/cvode-2.9.0/include/sundials/sundials_spbcgs.h"
    "C:/Users/robbr48/Documents/Mercurial/TLMPlugin/FMIWrapper/cvode-2.9.0/include/sundials/sundials_spfgmr.h"
    "C:/Users/robbr48/Documents/Mercurial/TLMPlugin/FMIWrapper/cvode-2.9.0/include/sundials/sundials_spgmr.h"
    "C:/Users/robbr48/Documents/Mercurial/TLMPlugin/FMIWrapper/cvode-2.9.0/include/sundials/sundials_sptfqmr.h"
    "C:/Users/robbr48/Documents/Mercurial/TLMPlugin/FMIWrapper/cvode-2.9.0/include/sundials/sundials_types.h"
    )
endif()

