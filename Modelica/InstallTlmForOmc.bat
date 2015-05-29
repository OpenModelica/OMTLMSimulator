rem /////////////////////SET PATHS////////////////////////////////////////////////////
rem set path to TLMPlugin (i.e. c:\TLMPlugin)
rem setx TLMPluginPath %BEAST%\src\TLMPlugin
rem set TLMPluginPath = %BEAST%\src\TLMPlugin
setx TLMPluginPath c:\SKF\TLMPlugin
set TLMPluginPath = c:\SKF\TLMPlugin

rem ///IMPORTANT!!!///
rem Add %TLMPluginPath%\bin path to the Windows System Environment Path.

rem set path to OpenModelica installation folder
set OpenModelicaPath=c:\OpenModelica1.9.1Nightly

rem set path to Microsoft Visual Studio installation folder
set VC32Path="c:\Program Files (x86)\Microsoft Visual Studio 10.0"

rem /////////////////////COMPILE AND INSTALL OPENMODELICA LIB////////////////////////
echo DOS BAT file to Mingw-make

rem create dir for libraries and include files 
mkdir %TLMPluginPath%\Modelica\Resources\Library
mkdir %TLMPluginPath%\Modelica\Resources\Include

rem Add the Modelica bin path to the Windows System Environment Variables:
rem reg add "HKLM\SYSTEM\CurrentControlSet\Control\Session Manager\Environment" /v Path /t REG_EXPAND_SZ /d "%TLMPluginPath%\bin;%path%" /f

rem This is needed to acces g++.exe
path=%OpenModelicaPath%\MinGW\bin;%PATH%
rem run mingw32-make.exe
set ABI=MINGW64
rem delete old MINGW64 folder
rmdir /s /q MINGW64
%OpenModelicaPath%\MinGW\bin\mingw32-make.exe -f MakeMingwLib
rem copy generated libtlmmodelica.a file to resource folder
copy %TLMPluginPath%\Modelica\MINGW64\libtlmmodelica.a %TLMPluginPath%\Modelica\Resources\Library\libtlmmodelica.a
rmdir /s /q MINGW64

echo DOS BAT file to vc++-make

rem set path to Microsoft Visual Studio installation folder
call %VC32Path%\Common7\Tools\vsvars32.bat 
rem run nmake.exe 
set ABI=VC32
rem delete old VC32 folder
rmdir /s /q VC32
nmake.exe -f MakeVcLib.vc default
rem copy generated tlmmodelica.lib file to the resource folder
copy %TLMPluginPath%\Modelica\%ABI%\tlmmodelica.lib %TLMPluginPath%\Modelica\Resources\Library
rem delete old VC32 folder
rmdir /s /q VC32

rem copy generated tlmforce.h filde to resource folder
copy %TLMPluginPath%\Modelica\tlmforce.h  %TLMPluginPath%\Modelica\Resources\Include

rem /////////////////////COMPILE AND INSTALL TLM MANAGER///////////////////////////////
rem set path to Microsoft Visual Studio installation folder
call %VC32Path%\Common7\Tools\vsvars32.bat 
call CompileTLMManager.bat