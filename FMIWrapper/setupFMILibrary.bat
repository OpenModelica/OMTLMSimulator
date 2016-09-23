@ECHO OFF
REM $Id: setupFMILibrary.bat 9022 2016-05-16 08:05:49Z petno25 $

REM Bat script building FMILibrary dependency automatically
REM Author: Peter Nordin peter.nordin@liu.se
REM Date:   2015-01-29
REM Modified for TLM framework by Robert Braun 2016-09-22

set arch=x64

set dirname=FMILibrary-2.0.1

REM Set tools paths depending on current arch
if defined ProgramFiles(x86) (
	REM do stuff for 64bit here
	echo 64bit Windows detected, expecting 64-bit tools
	set "cmake_path=%ProgramFiles%\CMake\bin"
) else (
    REM do stuff for 32bit here
	echo 32bit Windows detected, expecting 32-bit tools
	set "cmake_path=%ProgramFiles(x86)%\CMake\bin"
)

REM Set compilers and libs depending on selection
if "%arch%"=="x64" (
	set "mingw_path=C:\mingw32\bin"
) else (
	set "mingw_path=C:\mingw64\bin"
)
    
REM Automatic code starts here

REM Build
echo.
echo ======================
echo Building FMILibrary
echo ======================
set OLDPATH=%PATH%
set PATH=%mingw_path%;%cmake_path%;%OLDPATH%
cd FMILibrary-2.0.1
mkdir build-fmilib
cd build-fmilib
cmake -Wno-dev -G "MinGW Makefiles" -DFMILIB_FMI_PLATFORM="win64" ../
mingw32-make.exe -j4 -B
mingw32-make.exe install

echo.
echo Done
pause