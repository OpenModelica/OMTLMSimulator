@echo off
rem If other parameters are used for ADAMS, change the next line

set ADAMSSCRIPT=mdi ru-s

rem ***************************************************************
rem Changes below should not be necessary

if not a%1==a goto startadams
echo This script is used by TLM manager to start ADAMS simulations
echo It should result in a call to the mdi script
echo for running a simulation according to the specified ACF file
exit 1

:startadams

echo Starting an ADAMS simulation with input file: %1.acf
echo Make sure that: 
echo time = %2
echo timeEnd = %3
echo MaxTimeStep "<"= %4
echo Writing server configuration to file tlm.config

echo %1 > tlm.config
echo %2 >> tlm.config
echo %3 >> tlm.config
echo %4 >> tlm.config
echo %5 >> tlm.config

echo Starting ADAMS

%ADAMSSCRIPT% %1 > adams_out.txt
