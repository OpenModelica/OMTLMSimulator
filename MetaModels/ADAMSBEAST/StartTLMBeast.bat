@echo off
set SD=/home/iakov/ws/src/TLMPlugin/test/ADAMSBEAST
echo The execution directory on Linux is %SD%

set Host=gotx65
echo The execution host is set to %Host%

set User=iakov
echo The user name on Linux is set to %User%

echo Calling start script on %Host%
rsh %Host% -l %User% -n cd %SD% ; csh StartTLMBeast %*