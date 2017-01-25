echo on
REM OpenModelica TLM start-up script
REM Start with 6 arguments:
REM 1 XModelName (XModel directory) 
REM 2 start-time 
REM 3 end-time 
REM 4 max-time-step 
REM 5 server-name:port 
REM 6 FMU file

rem *********************SETUP SIMULATION*********************************************
set FMIWrapper_Cmd="FMIWrapper.exe" %CD%/%1 %6 solver=CVODE -d 

set TLMCONFIGFILE=tlm.config

If "%6" == "" goto USAGE

echo execution directory is %1
cd %1

If not exist %6 goto BADFILE

echo Starting an FMIWrapper simulation with input file: %6
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

echo Starting FMIWrapper
echo %FMIWrapper_Cmd%
echo DOING THIS: %FMIWrapper_Cmd% > %1.simlog
%FMIWrapper_Cmd%
echo DONE THIS: %FMIWrapper_Cmd% > %1.simlog

goto DONE

:USAGE
echo Usage: 
echo %0 XModelName start-time end-time max-time-step server-name:port model-file 
goto DONE


:BADFILE
echo Model file %6 not found, cannot start simulation


:DONE
echo Done %0