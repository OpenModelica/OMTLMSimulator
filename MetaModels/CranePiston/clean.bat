cd arm1
for %%i in (*.*) do if not "%%i"=="arm1.mo" if not "%%i"=="arm1.stl" del /q "%%i"
cd ..
cd arm2
for %%i in (*.*) do if not "%%i"=="arm2.mo" if not "%%i"=="arm2.stl" del /q "%%i"
cd ..
cd arm3
for %%i in (*.*) do if not "%%i"=="arm3.mo" if not "%%i"=="arm3.stl" del /q "%%i"
cd ..
cd controller
for %%i in (*.*) do if not "%%i"=="controller.hmf" del /q "%%i"
cd ..
cd controller_fmu
for %%i in (*.*) do if not "%%i"=="controller.fmu" if not "%%i"=="fmi.config" del /q "%%i"
cd ..
cd controller_simulink
for %%i in (*.*) do if not "%%i"=="controller.slx" del /q "%%i"
cd ..
cd dgbb1
for %%i in (*.*) do if not "%%i"=="dgbb.in" if not "%%i"=="dgbb.stl" del /q "%%i"
cd ..
cd dgbb2
for %%i in (*.*) do if not "%%i"=="dgbb.in" if not "%%i"=="dgbb.stl" del /q "%%i"
cd ..
cd hydraulics
for %%i in (*.*) do if not "%%i"=="hydraulics.hmf" del /q "%%i"
cd ..
cd load
for %%i in (*.*) do if not "%%i"=="load.mo" if not "%%i"=="load.stl" del /q "%%i"
cd ..
cd load_fmu
for %%i in (*.*) do if not "%%i"=="load.fmu" if not "%%i"=="fmi.config" if not "%%i"=="load.mo" if not "%%i"=="load.stl" del /q "%%i"
cd ..
cd piston
for %%i in (*.*) do if not "%%i"=="piston.mo" del /q "%%i"
cd ..
cd piston2
for %%i in (*.*) do if not "%%i"=="piston.mo" del /q "%%i"
cd ..
cd rope
for %%i in (*.*) do if not "%%i"=="rope.mo" if not "%%i"=="rope.stl" del /q "%%i"
cd ..
cd rope_fmu
for %%i in (*.*) do if not "%%i"=="rope.fmu" if not "%%i"=="fmi.conflig" if not "%%i"=="rope.mo" if not "%%i"=="rope.stl" del /q "%%i"
cd ..