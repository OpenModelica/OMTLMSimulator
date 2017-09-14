cd gain_fmu
g++ -fPIC -c gain.cpp -I"." 

gcc -fPIC -c gain_model_cs.c -I"/home/robbr48/Documents/Git/hopsan/Dependencies/FMILibrary/include"

g++ -fPIC -w -shared -static-libstdc++ -static-libgcc -Wl,--rpath,'$ORIGIN/.' gain.o gain_model_cs.o -o gain.so 

mkdir binaries
mkdir binaries/linux64

mv gain.so binaries/linux64

zip -r gain.fmu modelDescription.xml binaries/linux64/gain.so

rm -rf binaries
rm *.o

cp gain.fmu ../

cd ..

./fmuCheck.linux64 -n 5 gain.fmu
