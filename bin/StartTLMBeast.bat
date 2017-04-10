@echo off
set BeastCmd="%BEAST%/bin/%ABI%/FORMAT-%BEAST_FORMAT%/Beast_Serial"

echo execution directory is %1
cd %1

for /F %%i in ("%6") do set xname=%%~ni

echo Starting a Beast simulation with input file: %6
echo Make sure that: 
echo time = %2
echo timeEnd = %3
echo MaxTimeStep "<"= %4
echo Writing caseID %1 and server name %5 to file %xname%.tlm
echo %1 > %xname%.tlm 
echo %5 >> %xname%.tlm
echo %2 >> %xname%.tlm
echo %3 >> %xname%.tlm
echo %4 >> %xname%.tlm


echo Starting beast

echo %BeastCmd% %xname%.in

%BeastCmd% %xname%.in > %xname%.simlog

