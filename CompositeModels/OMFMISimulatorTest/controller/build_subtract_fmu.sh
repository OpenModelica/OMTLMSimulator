cd subtract_fmu
g++ -fPIC -c subtract.cpp -I"." 

gcc -fPIC -c subtract_model_cs.c -I"/home/robbr48/Documents/Git/hopsan/Dependencies/FMILibrary/include"

g++ -fPIC -w -shared -static-libstdc++ -static-libgcc -Wl,--rpath,'$ORIGIN/.' subtract.o subtract_model_cs.o -o subtract.so 

mkdir binaries
mkdir binaries/linux64

mv subtract.so binaries/linux64

zip -r subtract.fmu modelDescription.xml binaries/linux64/subtract.so

rm -rf binaries
rm *.o

cp subtract.fmu ../

cd ..

./fmuCheck.linux64 -n 5 subtract.fmu
