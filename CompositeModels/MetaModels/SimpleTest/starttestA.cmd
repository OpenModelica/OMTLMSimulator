@echo off
echo Starting testapp for i686-CYGWIN_NT-MV7, parameters:
echo 0 10 %*
"..\..\src\i686-CYGWIN_NT-MV7\testapp" 0 10 %* > %1.log
