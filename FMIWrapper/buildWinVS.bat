@ECHO OFF
REM Bat script building FMILibrary dependency automatically
REM Author: Robert Braun robert.braun@liu.se
REM Date:   2016-09-26
REM Note:   Based on scripts for Hopsan depencencies by Peter Nordin

REM Argument %1 should be architecture: x86 or x64

REM Default values. Use x64 for 64-bit, use x86 for 32-bit
set arch=x64

if not "%~1"=="" (
	set arch=%~2
)

echo Setting paths for Version: %1, Architecture: %arch%


set mingw_path32=C:\Qt\Tools\mingw491_32\bin
set mingw_path64=C:\Qt\x86_64-4.9.3-release-posix-seh-rt_v4-rev1\mingw64\bin


if not "%arch%"=="x86" (
  if not "%arch%"=="x64" (
	echo Error: arch must be x86 or x64
	pause
	goto :eof
  )
)

REM Tools paths
set cmake_path32=%ProgramFiles%\CMake\bin
set cmake_path64=%ProgramFiles(x86)%\CMake\bin
set msys_path=C:\msys64\usr\bin

REM Set tools paths depending on current arch
if defined ProgramFiles(x86) (
	REM do stuff for 64bit here
	echo 64bit Windows detected, expecting 64-bit tools
	set "cmake_path=%cmake_path64%"
) else (
    REM do stuff for 32bit here
	echo 32bit Windows detected, expecting 32-bit tools
	set "cmake_path=%cmake_path32%"
)

REM Set compilers and libs depending on selection
if "%arch%"=="x64" (
	set "mingw_path=%mingw_path64%"
) else (
	set "mingw_path=%mingw_path32%"
)

REM Echo resulting paths
echo CMake path:   %cmake_path%
echo Msys path:    %msys_path%
echo MinGW path:   %mingw_path%

echo.

set PATH=%mingw_path%;%cmake_path%;%PATH%

set dirname=FMILibrary-2.0.1

REM Build
echo.
echo ======================
echo Building 64-bit version of FMILibrary
echo ======================
cd %dirname%
mkdir build-fmilib
cd build-fmilib
cmake -Wno-dev -G "MinGW Makefiles" -DFMILIB_FMI_PLATFORM="win64" -DFMILIB_BUILD_TESTS:BOOL=0 -DFMILIB_GENERATE_DOXYGEN_DOC:BOOL=0 -DBUILD_TESTING:BOOL=0 -DFMILIB_BUILD_BEFORE_TESTS:BOOL=0 ../
mingw32-make.exe -j4 -B
mingw32-make.exe install

xcopy libfmilib_shared.dll ..\..\..\bin

echo.
echo Done
pause
