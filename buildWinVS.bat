@ECHO off

SET OMS_VS_TARGET=%~1
IF ["%OMS_VS_TARGET%"]==["VS14-Win32"] SET OMS_VS_PLATFORM=32 && SET OMS_VS_VERSION="Visual Studio 14 2015"
IF ["%OMS_VS_TARGET%"]==["VS14-Win64"] SET OMS_VS_PLATFORM=64 && SET OMS_VS_VERSION="Visual Studio 14 2015 Win64"
IF ["%OMS_VS_TARGET%"]==["VS15-Win32"] SET OMS_VS_PLATFORM=32 && SET OMS_VS_VERSION="Visual Studio 15 2017"
IF ["%OMS_VS_TARGET%"]==["VS15-Win64"] SET OMS_VS_PLATFORM=64 && SET OMS_VS_VERSION="Visual Studio 15 2017 Win64"

IF NOT DEFINED OMS_VS_VERSION (
  ECHO No argument or unsupported argument given. Use one of the following VS version strings:
  ECHO   "VS14-Win32" for Visual Studio 14 2015
  ECHO   "VS14-Win64" for Visual Studio 14 2015 Win64
  ECHO   "VS15-Win32" for Visual Studio 15 2017
  ECHO   "VS15-Win64" for Visual Studio 15 2017 Win64
  GOTO fail
)

SET OMS_VS_TARGET
SET OMS_VS_VERSION
SET OMS_VS_PLATFORM
ECHO.

SET VSCMD_START_DIR="%CD%"

IF ["%OMS_VS_TARGET%"]==["VS14-Win32"] @CALL "%VS140COMNTOOLS%\..\..\VC\vcvarsall.bat" x86
IF ["%OMS_VS_TARGET%"]==["VS14-Win64"] @CALL "%VS140COMNTOOLS%\..\..\VC\vcvarsall.bat" x86_amd64
IF ["%OMS_VS_TARGET%"]==["VS15-Win32"] @CALL "C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Auxiliary\Build\vcvarsall.bat" x86
IF ["%OMS_VS_TARGET%"]==["VS15-Win64"] @CALL "C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Auxiliary\Build\vcvarsall.bat" x86_amd64

IF NOT DEFINED CMAKE_BUILD_TYPE SET CMAKE_BUILD_TYPE="Release"
cd 3rdParty\misc
echo Building 3rdParty\misc
nmake -f Makefile.msvc
IF NOT ["%ERRORLEVEL%"]==["0"] GOTO fail
cd ..\rtime
echo Building 3rdParty\rtime
nmake -f Makefile.msvc
IF NOT ["%ERRORLEVEL%"]==["0"] GOTO fail
cd ..\..\common
echo Building common
nmake -f Makefile.msvc
IF NOT ["%ERRORLEVEL%"]==["0"] GOTO fail
cd ..\FMIWrapper\FMILibrary-2.0.1
mkdir build-fmilib
cd build-fmilib
cmake -DHAVE_SNPRINTF=1 -DHAVE_VSNPRINTF=1 -G %OMS_VS_VERSION% ../
cmake --build . --config MinSizeRel --target install
xcopy install/bin/fmilib_shared.dll ..\..\..\bin
xcopy install/bin/fmilib_shared.dll ..\..\..\..\lib
cd ../../
nmake -f Makefile.msvc
IF NOT ["%ERRORLEVEL%"]==["0"] GOTO fail
xcopy ..\..\bin\FMIWrapper ..\..\..\bin
cd ..

EXIT /B 0

:fail
ECHO OMTLMSimulator failed!
EXIT /B 1
