cd arm1
for %%i in (*.*) do if not "%%i"=="arm1.mo" if not "%%i"=="arm1.stl" del /q "%%i"
cd ..
cd arm2
for %%i in (*.*) do if not "%%i"=="arm2.mo" if not "%%i"=="arm2.stl" del /q "%%i"
cd ..
cd arm3
for %%i in (*.*) do if not "%%i"=="arm3.mo" if not "%%i"=="arm3.stl" del /q "%%i"
cd ..
cd load
for %%i in (*.*) do if not "%%i"=="load.mo" if not "%%i"=="load.stl" del /q "%%i"
cd ..
cd rope
for %%i in (*.*) do if not "%%i"=="rope.mo" if not "%%i"=="rope.stl" del /q "%%i"
cd ..
cd piston
for %%i in (*.*) do if not "%%i"=="piston.mo" del /q "%%i"
cd ..
cd piston2
for %%i in (*.*) do if not "%%i"=="piston.mo" del /q "%%i"
cd ..