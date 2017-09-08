echo off
REM OpenModelica TLM start-up script
REM Start with 6 arguments:
REM 1 XModelName (XModel directory) 
REM 2 start-time 
REM 3 end-time 
REM 4 max-time-step 
REM 5 server-name:port 
REM 6 model-file (without mo)

rem *********************SET PATHS****************************************************
rem set path to Dymola installation folder
set HopsanPath=%HOPSANHOME%

rem *********************SETUP SIMULATION*********************************************
rem use dir /x to study these weird MS-DOS paths 
set Hopsan_exe="%HopsanPath%\bin\HopsanCLI.exe"
set Hopsan_args=--resultsFullCSV hopsanlogdata.csv
set Hopsan_cmd=%Hopsan_exe% %Hopsan_args% -m %CD%/%1/%6 -e %TLMPluginPath%/Hopsan/TLMPluginLib/TLMPluginLib.dll -s %2,%4,%3 -l 1000
 
set TLMCONFIGFILE=tlm.config

If "%6" == "" goto USAGE

echo execution directory is %1
cd %1

echo Starting a Hopsan simulation with input file: %6
echo Make sure that: 
echo time = %2
echo timeEnd = %3
echo MaxTimeStep "<"= %4
echo 

echo Writing caseID %1 and server name %5 to file %TLMCONFIGFILE%
echo %1 > %TLMCONFIGFILE% 
echo %5 >> %TLMCONFIGFILE%
echo %2 >> %TLMCONFIGFILE%
echo %3 >> %TLMCONFIGFILE%
echo %4 >> %TLMCONFIGFILE%

echo Starting Hopsan
echo %Hopsan_Cmd%
echo DOING THIS: %Hopsan_Cmd% > %1.simlog
%Hopsan_Cmd% >> %1.simlog
echo DONE  THIS: %Hopsan_Cmd% >> %1.simlog

goto DONE

:USAGE
echo Usage: 
echo %0 XModelName start-time end-time max-time-step server-name:port model-file 
goto DONE


:BADFILE
echo Model file %6 not found, cannot start simulation


:DONE
echo Done %0