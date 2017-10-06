@echo off
rem If other parameters are used for Simulink, change the next line

rem ***************************************************************
rem Changes below should not be necessary

if not a%1==a goto startsim
echo This script is used by TLM manager to start Modelica simulations
echo It should result in a call to matlab/simulink
exit 1

:startsim

echo Simulation directory is %1
cd %1

set SIMCOMMAND="%MATLAB%/bin/matlab.exe" -nosplash -nodesktop -nojvm -minimize -r "cd %CD%; runtlm;"

echo Starting an Simulink simulation with: %6
echo Using default Simulink start script

echo ----- Writing server configuration to file tlm.config
echo %1 > tlm.config
echo %5 >> tlm.config
echo %2 >> tlm.config
echo %3 >> tlm.config
echo %4 >> tlm.config


echo ----- Write matlab runner script runtlm.m
echo addpath('%CD%'); > runtlm.m
echo addpath ('%TLMPluginPath%/Simulink'); > runtlm.m
echo %6; >> runtlm.m 

if "%4" EQU "0" goto noopts
:setopts
  echo opts = simset('MaxStep', %4); >> runtlm.m
  echo sim( '%6', [%2 %3], opts ); >> runtlm.m
  goto moveon

:noopts
  echo sim( '%6', [%2 %3] ); >> runtlm.m

:moveon
echo save; quit force; >> runtlm.m


echo Starting Simulink: %SIMCOMMAND%
%SIMCOMMAND% > %6.simlog
