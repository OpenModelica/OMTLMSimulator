rem /////////////////////COMPILE AND INSTALL CALC FUNCTION//////////////////////////////
cl %TLMPluginPath%\Calc\calc.c /FeAdvancedCalc
move %TLMPluginPath%\Modelica\AdvancedCalc.exe  %TLMPluginPath%\bin\

rem /////////////////////COMPILE AND INSTALL TLM MANAGER//////////////////////////////
rem run nmake.exe 
set ABI=WINDOWS32
nmake.exe -f MakeTlmManagerExe.vc lib
nmake.exe -f MakeTlmManagerExe.vc exe2
rem copy generated tlmmanager.exe file to Modelica bin folder
copy %TLMPluginPath%\Modelica\WINDOWS32\tlmmanager.exe %TLMPluginPath%\bin
nmake.exe -f MakeTlmManagerExe.vc clean
rmdir /s /q WINDOWS32

